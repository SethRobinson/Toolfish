#pragma once

#include <windows.h>
#include <tchar.h>

namespace ToastWindow
{
    // Call from any thread -- marshals to UI thread via PostMessage
    void ShowToast(const TCHAR* message);

    // Called on the UI thread by the WM_SHOW_TOAST handler
    void ShowToastOnUIThread(const TCHAR* message);
};
