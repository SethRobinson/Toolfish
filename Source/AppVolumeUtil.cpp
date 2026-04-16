#include "stdafx.h"
#include "AppVolumeUtil.h"
#include "CGlobals.h"

#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <tlhelp32.h>

static const GUID IID_IAudioSessionManager2_local = 
    { 0x77AA99A0, 0x1BD6, 0x484F, {0x8B, 0xC7, 0x2C, 0x65, 0x4C, 0x9A, 0x9B, 0x6F} };
static const GUID IID_IAudioSessionControl2_local = 
    { 0xbfb7ff88, 0x7239, 0x4fc9, {0x8f, 0xa2, 0x07, 0xc9, 0x50, 0xbe, 0x9c, 0x6d} };
static const GUID IID_ISimpleAudioVolume_local = 
    { 0x87CE5498, 0x68D6, 0x44E5, {0x92, 0x15, 0x6D, 0xA4, 0x7E, 0xF8, 0x83, 0xD8} };

static void GetProcessName(DWORD pid, TCHAR* nameOut, int nameSize)
{
    if (!nameOut || nameSize <= 0) return;
    nameOut[0] = _T('\0');

    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProc) return;

    TCHAR fullPath[MAX_PATH] = {0};
    DWORD pathSize = MAX_PATH;
    if (QueryFullProcessImageName(hProc, 0, fullPath, &pathSize))
    {
        TCHAR* slash = _tcsrchr(fullPath, _T('\\'));
        TCHAR* name = slash ? slash + 1 : fullPath;
        TCHAR* dot = _tcsrchr(name, _T('.'));
        int nameLen = dot ? (int)(dot - name) : (int)_tcslen(name);
        if (nameLen >= nameSize) nameLen = nameSize - 1;
        _tcsncpy(nameOut, name, nameLen);
        nameOut[nameLen] = _T('\0');
    }
    CloseHandle(hProc);
}

// Collect all PIDs that are children (or grandchildren, etc.) of parentPid
static void CollectChildPids(DWORD parentPid, DWORD* pids, int maxPids, int& count)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(pe);

    if (Process32First(hSnap, &pe))
    {
        do
        {
            if (pe.th32ParentProcessID == parentPid && pe.th32ProcessID != parentPid)
            {
                if (count < maxPids)
                    pids[count++] = pe.th32ProcessID;
            }
        } while (Process32Next(hSnap, &pe));
    }
    CloseHandle(hSnap);
}

// Try to find and adjust volume for any PID in the set; returns result percent or -1
static int TrySetVolumeForPids(IAudioSessionEnumerator* pSessionEnum,
                               const DWORD* pids, int pidCount,
                               int volume, bool relative, DWORD* matchedPid)
{
    int sessionCount = 0;
    pSessionEnum->GetCount(&sessionCount);

    for (int i = 0; i < sessionCount; i++)
    {
        IAudioSessionControl* pCtl = NULL;
        IAudioSessionControl2* pCtl2 = NULL;
        ISimpleAudioVolume* pSimpleVol = NULL;

        HRESULT hr = pSessionEnum->GetSession(i, &pCtl);
        if (FAILED(hr) || !pCtl) continue;

        hr = pCtl->QueryInterface(IID_IAudioSessionControl2_local, (void**)&pCtl2);
        if (FAILED(hr) || !pCtl2)
        {
            pCtl->Release();
            continue;
        }

        DWORD sessionPid = 0;
        pCtl2->GetProcessId(&sessionPid);

        bool match = false;
        for (int p = 0; p < pidCount; p++)
        {
            if (sessionPid == pids[p])
            {
                match = true;
                break;
            }
        }

        if (match)
        {
            hr = pCtl->QueryInterface(IID_ISimpleAudioVolume_local, (void**)&pSimpleVol);
            if (SUCCEEDED(hr) && pSimpleVol)
            {
                float newLevel;
                if (relative)
                {
                    float currentLevel = 0.0f;
                    pSimpleVol->GetMasterVolume(&currentLevel);
                    newLevel = currentLevel + (float)volume / 100.0f;
                }
                else
                {
                    newLevel = (float)volume / 100.0f;
                }

                if (newLevel < 0.0f) newLevel = 0.0f;
                if (newLevel > 1.0f) newLevel = 1.0f;

                pSimpleVol->SetMasterVolume(newLevel, NULL);
                int resultPercent = (int)(newLevel * 100.0f + 0.5f);

                if (matchedPid) *matchedPid = sessionPid;

                pSimpleVol->Release();
                pCtl2->Release();
                pCtl->Release();
                return resultPercent;
            }
        }

        pCtl2->Release();
        pCtl->Release();
    }

    return -1;
}

int SetActiveWindowVolume(int volume, bool relative, TCHAR* appNameOut, int appNameSize)
{
    if (appNameOut && appNameSize > 0)
        appNameOut[0] = _T('\0');

    HWND hwndFG = GetForegroundWindow();
    if (!hwndFG)
    {
        LogMsg(_T("App volume: No foreground window found."));
        return -1;
    }

    DWORD targetPid = 0;
    GetWindowThreadProcessId(hwndFG, &targetPid);
    if (targetPid == 0)
    {
        LogMsg(_T("App volume: Could not get PID of foreground window."));
        return -1;
    }

    CoInitialize(NULL);

    HRESULT hr;
    int resultPercent = -1;

    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioSessionManager2* pSessionMgr = NULL;
    IAudioSessionEnumerator* pSessionEnum = NULL;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                          __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr) || !pEnumerator)
    {
        LogMsg(_T("App volume: Failed to create device enumerator."));
        goto Cleanup;
    }

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr) || !pDevice)
    {
        LogMsg(_T("App volume: No default audio output device."));
        goto Cleanup;
    }

    hr = pDevice->Activate(IID_IAudioSessionManager2_local, CLSCTX_ALL, NULL, (void**)&pSessionMgr);
    if (FAILED(hr) || !pSessionMgr)
    {
        LogMsg(_T("App volume: Failed to get audio session manager."));
        goto Cleanup;
    }

    hr = pSessionMgr->GetSessionEnumerator(&pSessionEnum);
    if (FAILED(hr) || !pSessionEnum)
    {
        LogMsg(_T("App volume: Failed to enumerate audio sessions."));
        goto Cleanup;
    }

    {
        // First try the foreground window's own PID
        DWORD matchedPid = 0;
        DWORD pidsToTry[1] = { targetPid };
        resultPercent = TrySetVolumeForPids(pSessionEnum, pidsToTry, 1,
                                            volume, relative, &matchedPid);

        // If no match, try child processes (handles Chrome, Firefox, etc.)
        if (resultPercent < 0)
        {
            DWORD childPids[256];
            int childCount = 0;
            CollectChildPids(targetPid, childPids, 256, childCount);

            if (childCount > 0)
            {
                resultPercent = TrySetVolumeForPids(pSessionEnum, childPids, childCount,
                                                    volume, relative, &matchedPid);
            }
        }

        if (resultPercent >= 0)
        {
            // Use the foreground window's process name for display
            GetProcessName(targetPid, appNameOut, appNameSize);
        }
        else
        {
            LogMsg(_T("App volume: No audio session found for the foreground window (PID %d)."), targetPid);
        }
    }

Cleanup:
    if (pSessionEnum) pSessionEnum->Release();
    if (pSessionMgr) pSessionMgr->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();

    return resultPercent;
}
