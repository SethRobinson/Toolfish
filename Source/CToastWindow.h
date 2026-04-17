#pragma once

#include <windows.h>
#include <tchar.h>

namespace ToastWindow
{
    struct Options
    {
        HWND anchorWindow;   // NULL = center on monitor of foreground window
        int  progress;       // -1 = no meter, 0-100 = show meter at this percent

        Options() : anchorWindow(NULL), progress(-1) {}
    };

    // Plain text toast (no meter, monitor-centered). Safe from any thread.
    void ShowToast(const TCHAR* message);

    // Toast with options (anchor window + optional meter). Safe from any thread.
    void ShowToast(const TCHAR* message, const Options& opts);

    // Called on the UI thread by the WM_SHOW_TOAST handler
    void ShowToastOnUIThread(const TCHAR* message, const Options& opts);

    // Helper: invoked from the WM_SHOW_TOAST window message dispatcher.
    // Casts the lParam payload (allocated by ShowToast) and calls
    // ShowToastOnUIThread, then frees the payload.
    void HandleShowToastMessage(LPARAM lParam);
};
