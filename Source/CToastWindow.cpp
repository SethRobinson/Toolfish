#include "stdafx.h"
#include "CToastWindow.h"
#include "CGlobals.h"

static const TCHAR* TOAST_CLASS_NAME = _T("ToolfishToastWnd");
static const UINT TOAST_TIMER_ID = 42;
static const int FADE_IN_MS = 200;
static const int HOLD_MS = 1500;
static const int FADE_OUT_MS = 300;
static const int MAX_ALPHA = 210;
static const int TOAST_WIDTH = 340;
static const int TOAST_HEIGHT = 40;

enum ToastPhase { PHASE_FADE_IN, PHASE_HOLD, PHASE_FADE_OUT };

struct ToastData
{
    TCHAR message[256];
    ToastPhase phase;
    int alpha;
    DWORD dwPhaseStart;
    HFONT hFont;
};

static HWND s_hwndToast = NULL;
static bool s_classRegistered = false;

static void DestroyCurrentToast()
{
    if (s_hwndToast && ::IsWindow(s_hwndToast))
        ::DestroyWindow(s_hwndToast);
    s_hwndToast = NULL;
}

static LRESULT CALLBACK ToastWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        ToastData* pData = (ToastData*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        PAINTSTRUCT ps;
        HDC hdc = ::BeginPaint(hwnd, &ps);
        RECT rc;
        ::GetClientRect(hwnd, &rc);

        HBRUSH hBrush = ::CreateSolidBrush(RGB(30, 30, 30));
        ::FillRect(hdc, &rc, hBrush);
        ::DeleteObject(hBrush);

        if (pData)
        {
            HFONT hOldFont = (HFONT)::SelectObject(hdc, pData->hFont);
            ::SetTextColor(hdc, RGB(240, 240, 240));
            ::SetBkMode(hdc, TRANSPARENT);
            ::DrawText(hdc, pData->message, -1, &rc,
                       DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
            ::SelectObject(hdc, hOldFont);
        }

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
            if (pData->hFont) ::DeleteObject(pData->hFont);
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

    if (::RegisterClass(&wc))
        s_classRegistered = true;
}

void ToastWindow::ShowToast(const TCHAR* message)
{
    if (!message || !message[0])
        return;

    int len = (int)_tcslen(message) + 1;
    TCHAR* copy = new TCHAR[len];
    _tcscpy(copy, message);
    ::PostMessage(app_glo.GetHWND(), WM_SHOW_TOAST, 0, (LPARAM)copy);
}

void ToastWindow::ShowToastOnUIThread(const TCHAR* message)
{
    DestroyCurrentToast();
    RegisterToastClass();

    if (!s_classRegistered)
        return;

    HMONITOR hMon = NULL;
    HWND hwndFG = ::GetForegroundWindow();
    if (hwndFG)
        hMon = ::MonitorFromWindow(hwndFG, MONITOR_DEFAULTTONEAREST);
    if (!hMon)
        hMon = ::MonitorFromPoint({0, 0}, MONITOR_DEFAULTTOPRIMARY);

    MONITORINFO mi = { sizeof(mi) };
    ::GetMonitorInfo(hMon, &mi);

    int workW = mi.rcWork.right - mi.rcWork.left;
    int workH = mi.rcWork.bottom - mi.rcWork.top;
    int x = mi.rcWork.left + (workW - TOAST_WIDTH) / 2;
    int y = mi.rcWork.top + (workH * 2 / 3);

    DWORD exStyle = WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;

    HWND hwnd = ::CreateWindowEx(exStyle, TOAST_CLASS_NAME, _T(""),
                                 WS_POPUP, x, y, TOAST_WIDTH, TOAST_HEIGHT,
                                 NULL, NULL, ::GetModuleHandle(NULL), NULL);
    if (!hwnd)
        return;

    ToastData* pData = new ToastData();
    _tcsncpy(pData->message, message, 255);
    pData->message[255] = _T('\0');
    pData->phase = PHASE_FADE_IN;
    pData->alpha = 0;
    pData->dwPhaseStart = ::GetTickCount();
    pData->hFont = ::CreateFont(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Segoe UI"));

    ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pData);
    ::SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
    ::ShowWindow(hwnd, SW_SHOWNOACTIVATE);
    ::SetTimer(hwnd, TOAST_TIMER_ID, 30, NULL);

    s_hwndToast = hwnd;
}
