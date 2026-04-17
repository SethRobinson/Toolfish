#include "stdafx.h"
#include "CToastWindow.h"
#include "CGlobals.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

static const TCHAR* TOAST_CLASS_NAME = _T("ToolfishToastWnd");
static const UINT TOAST_TIMER_ID = 42;
static const int FADE_IN_MS = 200;
static const int HOLD_MS = 1500;
static const int FADE_OUT_MS = 300;
static const int MAX_ALPHA = 230;

static const int TOAST_WIDTH      = 460;
static const int TOAST_HEIGHT_TXT = 64;   // text-only toast
static const int TOAST_HEIGHT_BAR = 116;  // toast with meter bar
static const int CORNER_RADIUS    = 14;

enum ToastPhase { PHASE_FADE_IN, PHASE_HOLD, PHASE_FADE_OUT };

struct ToastData
{
    TCHAR message[256];
    int progress;          // -1 = no meter, 0-100 otherwise
    ToastPhase phase;
    int alpha;
    DWORD dwPhaseStart;
    HFONT hFontMain;
    HFONT hFontPercent;
};

// Marshalled across threads via WM_SHOW_TOAST
struct ToastPayload
{
    TCHAR message[256];
    HWND  anchorWindow;
    int   progress;
};

static HWND s_hwndToast = NULL;
static bool s_classRegistered = false;

static void DestroyCurrentToast()
{
    if (s_hwndToast && ::IsWindow(s_hwndToast))
        ::DestroyWindow(s_hwndToast);
    s_hwndToast = NULL;
}

static void PaintMeterBar(HDC hdc, const RECT& barRect, int progress, HFONT hFontPercent)
{
    int p = progress;
    if (p < 0)   p = 0;
    if (p > 100) p = 100;

    int barW = barRect.right - barRect.left;

    HBRUSH hTrack = ::CreateSolidBrush(RGB(60, 60, 60));
    ::FillRect(hdc, &barRect, hTrack);
    ::DeleteObject(hTrack);

    COLORREF accent;
    if (p <= 70)
        accent = RGB(0, 160, 255);
    else if (p <= 90)
        accent = RGB(80, 200, 120);
    else
        accent = RGB(255, 160, 40);

    if (p > 0)
    {
        RECT fillRect = barRect;
        fillRect.right = barRect.left + (barW * p) / 100;
        HBRUSH hFill = ::CreateSolidBrush(accent);
        ::FillRect(hdc, &fillRect, hFill);
        ::DeleteObject(hFill);
    }

    HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(90, 90, 90));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
    ::Rectangle(hdc, barRect.left, barRect.top, barRect.right, barRect.bottom);
    ::SelectObject(hdc, hOldBrush);
    ::SelectObject(hdc, hOldPen);
    ::DeleteObject(hPen);

    if (hFontPercent)
    {
        TCHAR pct[16];
        _stprintf(pct, _T("%d%%"), p);

        HFONT hOldFont = (HFONT)::SelectObject(hdc, hFontPercent);
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, RGB(245, 245, 245));

        RECT textRect = barRect;
        textRect.right -= 8;
        ::DrawText(hdc, pct, -1, &textRect,
                   DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
        ::SelectObject(hdc, hOldFont);
    }
}

// Paint the entire toast contents to the supplied DC.
static void DrawToastContents(HDC hdc, const RECT& rc, ToastData* pData)
{
    HBRUSH hBrush = ::CreateSolidBrush(RGB(28, 28, 30));
    ::FillRect(hdc, &rc, hBrush);
    ::DeleteObject(hBrush);

    HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(70, 70, 75));
    HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
    ::Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    ::SelectObject(hdc, hOldBrush);
    ::SelectObject(hdc, hOldPen);
    ::DeleteObject(hPen);

    if (!pData)
        return;

    bool hasMeter = (pData->progress >= 0);

    RECT textRect = rc;
    ::InflateRect(&textRect, -16, 0);

    if (hasMeter)
    {
        textRect.top    += 10;
        textRect.bottom  = rc.top + (rc.bottom - rc.top) / 2 + 6;
    }

    HFONT hOldFont = (HFONT)::SelectObject(hdc, pData->hFontMain);
    ::SetTextColor(hdc, RGB(240, 240, 240));
    ::SetBkMode(hdc, TRANSPARENT);
    ::DrawText(hdc, pData->message, -1, &textRect,
               DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    ::SelectObject(hdc, hOldFont);

    if (hasMeter)
    {
        RECT barRect;
        barRect.left   = rc.left + 24;
        barRect.right  = rc.right - 24;
        barRect.bottom = rc.bottom - 16;
        barRect.top    = barRect.bottom - 22;
        PaintMeterBar(hdc, barRect, pData->progress, pData->hFontPercent);
    }
}

static LRESULT CALLBACK ToastWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    // Skip the default background erase to avoid the flash before WM_PAINT.
    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT:
    {
        ToastData* pData = (ToastData*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(hwnd, &ps);
        RECT rc;
        ::GetClientRect(hwnd, &rc);

        // Double-buffered paint: render to a memory DC first, then BitBlt.
        // This eliminates the flicker caused by drawing each piece (background,
        // border, text, meter) directly onto the visible window.
        HDC     memDC  = ::CreateCompatibleDC(hdc);
        HBITMAP memBmp = ::CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
        HBITMAP oldBmp = (HBITMAP)::SelectObject(memDC, memBmp);

        DrawToastContents(memDC, rc, pData);

        ::BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                 memDC, 0, 0, SRCCOPY);

        ::SelectObject(memDC, oldBmp);
        ::DeleteObject(memBmp);
        ::DeleteDC(memDC);

        ::EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_TIMER:
    {
        if (wParam == TOAST_TIMER_ID)
        {
            ToastData* pData = (ToastData*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
            if (!pData)
                return 0;

            DWORD elapsed = ::GetTickCount() - pData->dwPhaseStart;

            switch (pData->phase)
            {
            case PHASE_FADE_IN:
                pData->alpha = (int)(MAX_ALPHA * min(1.0f, (float)elapsed / FADE_IN_MS));
                if (elapsed >= (DWORD)FADE_IN_MS)
                {
                    pData->alpha = MAX_ALPHA;
                    pData->phase = PHASE_HOLD;
                    pData->dwPhaseStart = ::GetTickCount();
                }
                break;

            case PHASE_HOLD:
                pData->alpha = MAX_ALPHA;
                if (elapsed >= (DWORD)HOLD_MS)
                {
                    pData->phase = PHASE_FADE_OUT;
                    pData->dwPhaseStart = ::GetTickCount();
                }
                break;

            case PHASE_FADE_OUT:
                pData->alpha = MAX_ALPHA - (int)(MAX_ALPHA * min(1.0f, (float)elapsed / FADE_OUT_MS));
                if (elapsed >= (DWORD)FADE_OUT_MS)
                {
                    ::DestroyWindow(hwnd);
                    return 0;
                }
                break;
            }

            ::SetLayeredWindowAttributes(hwnd, 0, (BYTE)pData->alpha, LWA_ALPHA);
            return 0;
        }
        break;
    }

    case WM_NCHITTEST:
        return HTTRANSPARENT;

    case WM_DESTROY:
    {
        ::KillTimer(hwnd, TOAST_TIMER_ID);
        ToastData* pData = (ToastData*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        ::SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        if (pData)
        {
            if (pData->hFontMain)    ::DeleteObject(pData->hFontMain);
            if (pData->hFontPercent) ::DeleteObject(pData->hFontPercent);
            delete pData;
        }
        if (s_hwndToast == hwnd)
            s_hwndToast = NULL;
        return 0;
    }
    }

    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

static void RegisterToastClass()
{
    if (s_classRegistered)
        return;

    WNDCLASS wc = {0};
    wc.lpfnWndProc = ToastWndProc;
    wc.hInstance = ::GetModuleHandle(NULL);
    wc.lpszClassName = TOAST_CLASS_NAME;
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    // No background brush -- we paint everything ourselves and handle WM_ERASEBKGND.
    wc.hbrBackground = NULL;
    // CS_HREDRAW / CS_VREDRAW intentionally omitted: we control invalidation.

    if (::RegisterClass(&wc))
        s_classRegistered = true;
}

// Compute toast position. Returns x,y for the toast window.
// If anchorWindow is valid, centers over that window (clamped to its monitor's work area).
// Otherwise, falls back to centering on the foreground window's monitor at 2/3 down.
static void ComputeToastPosition(HWND anchorWindow, int toastW, int toastH,
                                 int& outX, int& outY)
{
    RECT anchorRect = {0};
    bool haveAnchor = false;

    if (anchorWindow && ::IsWindow(anchorWindow) && !::IsIconic(anchorWindow))
    {
        if (FAILED(::DwmGetWindowAttribute(anchorWindow, DWMWA_EXTENDED_FRAME_BOUNDS,
                                           &anchorRect, sizeof(anchorRect))))
        {
            ::GetWindowRect(anchorWindow, &anchorRect);
        }
        if (anchorRect.right > anchorRect.left && anchorRect.bottom > anchorRect.top)
            haveAnchor = true;
    }

    HMONITOR hMon = NULL;
    if (haveAnchor)
        hMon = ::MonitorFromWindow(anchorWindow, MONITOR_DEFAULTTONEAREST);
    else
    {
        HWND hwndFG = ::GetForegroundWindow();
        if (hwndFG)
            hMon = ::MonitorFromWindow(hwndFG, MONITOR_DEFAULTTONEAREST);
    }
    if (!hMon)
        hMon = ::MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY);

    MONITORINFO mi = { sizeof(mi) };
    ::GetMonitorInfo(hMon, &mi);

    int x, y;
    if (haveAnchor)
    {
        int aw = anchorRect.right - anchorRect.left;
        int ah = anchorRect.bottom - anchorRect.top;
        x = anchorRect.left + (aw - toastW) / 2;
        y = anchorRect.top  + (ah - toastH) / 2;
    }
    else
    {
        int workW = mi.rcWork.right - mi.rcWork.left;
        int workH = mi.rcWork.bottom - mi.rcWork.top;
        x = mi.rcWork.left + (workW - toastW) / 2;
        y = mi.rcWork.top  + (workH * 2 / 3);
    }

    if (x < mi.rcWork.left) x = mi.rcWork.left;
    if (y < mi.rcWork.top)  y = mi.rcWork.top;
    if (x + toastW > mi.rcWork.right)  x = mi.rcWork.right  - toastW;
    if (y + toastH > mi.rcWork.bottom) y = mi.rcWork.bottom - toastH;

    outX = x;
    outY = y;
}

void ToastWindow::ShowToast(const TCHAR* message)
{
    Options opts;
    ShowToast(message, opts);
}

void ToastWindow::HandleShowToastMessage(LPARAM lParam)
{
    ToastPayload* payload = (ToastPayload*)lParam;
    if (!payload)
        return;

    Options opts;
    opts.anchorWindow = payload->anchorWindow;
    opts.progress     = payload->progress;
    ShowToastOnUIThread(payload->message, opts);
    delete payload;
}

void ToastWindow::ShowToast(const TCHAR* message, const Options& opts)
{
    if (!message || !message[0])
        return;

    ToastPayload* payload = new ToastPayload();
    _tcsncpy(payload->message, message, 255);
    payload->message[255] = _T('\0');
    payload->anchorWindow = opts.anchorWindow;
    payload->progress     = opts.progress;

    ::PostMessage(app_glo.GetHWND(), WM_SHOW_TOAST, 0, (LPARAM)payload);
}

void ToastWindow::ShowToastOnUIThread(const TCHAR* message, const Options& opts)
{
    RegisterToastClass();
    if (!s_classRegistered)
        return;

    bool hasMeter = (opts.progress >= 0);
    int toastW = TOAST_WIDTH;
    int toastH = hasMeter ? TOAST_HEIGHT_BAR : TOAST_HEIGHT_TXT;

    int x = 0, y = 0;
    ComputeToastPosition(opts.anchorWindow, toastW, toastH, x, y);

    // Reuse the existing toast window if one is already showing. This
    // eliminates the flicker caused by destroying + recreating the layered
    // window when the user spins the volume wheel rapidly.
    if (s_hwndToast && ::IsWindow(s_hwndToast))
    {
        HWND hwnd = s_hwndToast;
        ToastData* pData = (ToastData*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (pData)
        {
            _tcsncpy(pData->message, message, 255);
            pData->message[255] = _T('\0');
            pData->progress     = opts.progress;
            // Snap straight to fully-visible HOLD phase so updates feel instant.
            pData->phase        = PHASE_HOLD;
            pData->alpha        = MAX_ALPHA;
            pData->dwPhaseStart = ::GetTickCount();
        }

        // Resize/move (and refresh the rounded-corner region if size changed).
        RECT curRect;
        ::GetWindowRect(hwnd, &curRect);
        int curW = curRect.right - curRect.left;
        int curH = curRect.bottom - curRect.top;

        ::SetWindowPos(hwnd, HWND_TOPMOST, x, y, toastW, toastH,
                       SWP_NOACTIVATE | SWP_SHOWWINDOW);

        if (curW != toastW || curH != toastH)
        {
            HRGN hRgn = ::CreateRoundRectRgn(0, 0, toastW + 1, toastH + 1,
                                             CORNER_RADIUS, CORNER_RADIUS);
            if (hRgn)
                ::SetWindowRgn(hwnd, hRgn, TRUE);
        }

        ::SetLayeredWindowAttributes(hwnd, 0, (BYTE)MAX_ALPHA, LWA_ALPHA);
        ::InvalidateRect(hwnd, NULL, FALSE);
        ::UpdateWindow(hwnd);
        return;
    }

    DWORD exStyle = WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;

    HWND hwnd = ::CreateWindowEx(exStyle, TOAST_CLASS_NAME, _T(""),
                                 WS_POPUP, x, y, toastW, toastH,
                                 NULL, NULL, ::GetModuleHandle(NULL), NULL);
    if (!hwnd)
        return;

    HRGN hRgn = ::CreateRoundRectRgn(0, 0, toastW + 1, toastH + 1,
                                     CORNER_RADIUS, CORNER_RADIUS);
    if (hRgn)
        ::SetWindowRgn(hwnd, hRgn, TRUE);

    ToastData* pData = new ToastData();
    _tcsncpy(pData->message, message, 255);
    pData->message[255] = _T('\0');
    pData->progress     = opts.progress;
    pData->phase        = PHASE_FADE_IN;
    pData->alpha        = 0;
    pData->dwPhaseStart = ::GetTickCount();
    pData->hFontMain    = ::CreateFont(28, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                       CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));
    pData->hFontPercent = ::CreateFont(14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                       CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));

    ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);

    // Start fully transparent and force the first paint BEFORE making the
    // window visible composition-wise. This avoids a one-frame flash of
    // unpainted content when the layered window first appears.
    ::SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
    ::ShowWindow(hwnd, SW_SHOWNOACTIVATE);
    ::UpdateWindow(hwnd);

    ::SetTimer(hwnd, TOAST_TIMER_ID, 30, NULL);

    s_hwndToast = hwnd;
}
