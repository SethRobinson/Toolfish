#include "stdafx.h"
#include "registry_util.h"
#include "CGlobals.h"
#include "..\Shared\registry.h"
#include <taskschd.h>
#include <comdef.h>
#include <sddl.h>  // For ConvertSidToStringSid
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

// Delete the Toolfish scheduled task if it exists
bool DeleteAdminStartupTask()
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
    {
        log_error("DeleteAdminStartupTask: CoInitialize failed");
        return false;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
                          IID_ITaskService, (void**)&pService);
    if (FAILED(hr))
    {
        CoUninitialize();
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        pService->Release();
        CoUninitialize();
        return false;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr))
    {
        pService->Release();
        CoUninitialize();
        return false;
    }

    // Delete the task - this will fail silently if it doesn't exist
    hr = pRootFolder->DeleteTask(_bstr_t(L"Toolfish"), 0);

    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    return SUCCEEDED(hr);
}

// Get the actual logged-in user's SID string (for accessing their registry when elevated)
// Returns empty string on failure
CString GetLoggedInUserSID()
{
    CString sidString;
    
    // Get the user from the explorer.exe process (always runs as the logged-in user)
    DWORD explorerPid = 0;
    HWND shellWnd = GetShellWindow();
    if (shellWnd) {
        GetWindowThreadProcessId(shellWnd, &explorerPid);
    }
    
    if (explorerPid != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, explorerPid);
        if (hProcess) {
            HANDLE hToken;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                DWORD dwSize = 0;
                GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
                if (dwSize > 0) {
                    PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
                    if (pTokenUser && GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
                        LPTSTR stringSid = NULL;
                        if (ConvertSidToStringSid(pTokenUser->User.Sid, &stringSid)) {
                            sidString = stringSid;
                            LocalFree(stringSid);
                        }
                    }
                    free(pTokenUser);
                }
                CloseHandle(hToken);
            }
            CloseHandle(hProcess);
        }
    }
    
    return sidString;
}

// Get the actual logged-in user's name
CString GetLoggedInUserName()
{
    CString userName;
    
    DWORD explorerPid = 0;
    HWND shellWnd = GetShellWindow();
    if (shellWnd) {
        GetWindowThreadProcessId(shellWnd, &explorerPid);
    }
    
    if (explorerPid != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, explorerPid);
        if (hProcess) {
            HANDLE hToken;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                DWORD dwSize = 0;
                GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
                if (dwSize > 0) {
                    PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
                    if (pTokenUser && GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
                        TCHAR name[256], domain[256];
                        DWORD nameLen = 256, domainLen = 256;
                        SID_NAME_USE sidType;
                        if (LookupAccountSid(NULL, pTokenUser->User.Sid, name, &nameLen, domain, &domainLen, &sidType)) {
                            userName = name;
                        }
                    }
                    free(pTokenUser);
                }
                CloseHandle(hToken);
            }
            CloseHandle(hProcess);
        }
    }
    
    if (userName.IsEmpty()) {
        TCHAR envUser[256];
        if (GetEnvironmentVariable(_T("USERNAME"), envUser, 256) > 0) {
            userName = envUser;
        }
    }
    
    return userName;
}

// Check if the logged-in user (not the elevated account) is a member of Administrators group
// Note: With UAC, admin users have a filtered token where the admin SID is present but disabled.
// We check if the SID is present at all (not just enabled) to detect actual admin membership.
bool IsLoggedInUserAdmin()
{
    bool isAdmin = false;
    
    // Get the user from explorer.exe (the actual logged-in user)
    DWORD explorerPid = 0;
    HWND shellWnd = GetShellWindow();
    if (shellWnd) {
        GetWindowThreadProcessId(shellWnd, &explorerPid);
    }
    
    if (explorerPid != 0) {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, explorerPid);
        if (hProcess) {
            HANDLE hToken;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                // Create admin SID
                SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
                PSID AdminGroup;
                if (AllocateAndInitializeSid(&NtAuthority, 2,
                    SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                    0, 0, 0, 0, 0, 0, &AdminGroup))
                {
                    // Get token groups to check if admin SID is present (even if disabled by UAC)
                    DWORD dwSize = 0;
                    GetTokenInformation(hToken, TokenGroups, NULL, 0, &dwSize);
                    if (dwSize > 0) {
                        PTOKEN_GROUPS pGroups = (PTOKEN_GROUPS)malloc(dwSize);
                        if (pGroups && GetTokenInformation(hToken, TokenGroups, pGroups, dwSize, &dwSize)) {
                            for (DWORD i = 0; i < pGroups->GroupCount; i++) {
                                if (EqualSid(pGroups->Groups[i].Sid, AdminGroup)) {
                                    isAdmin = true;
                                    break;
                                }
                            }
                        }
                        free(pGroups);
                    }
                    FreeSid(AdminGroup);
                }
                CloseHandle(hToken);
            }
            CloseHandle(hProcess);
        }
    }
    
    return isAdmin;
}

// Create a scheduled task that runs Toolfish at logon with highest privileges (admin)
bool CreateAdminStartupTask(const CString& exePath)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
    {
        log_error("CreateAdminStartupTask: CoInitialize failed");
        return false;
    }
    
    // Get the actual logged-in user (not the elevated admin)
    CString loggedInUser = GetLoggedInUserName();

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
                          IID_ITaskService, (void**)&pService);
    if (FAILED(hr))
    {
        log_error("CreateAdminStartupTask: Failed to create TaskScheduler instance");
        CoUninitialize();
        return false;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr))
    {
        log_error("CreateAdminStartupTask: Failed to connect to Task Scheduler");
        pService->Release();
        CoUninitialize();
        return false;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr))
    {
        log_error("CreateAdminStartupTask: Failed to get root folder");
        pService->Release();
        CoUninitialize();
        return false;
    }

    // Delete existing task if present
    pRootFolder->DeleteTask(_bstr_t(L"Toolfish"), 0);

    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr))
    {
        log_error("CreateAdminStartupTask: Failed to create task definition");
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return false;
    }

    // Get registration info and set author
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (SUCCEEDED(hr))
    {
        pRegInfo->put_Author(_bstr_t(L"Toolfish"));
        pRegInfo->put_Description(_bstr_t(L"Starts Toolfish at logon with administrator privileges"));
        pRegInfo->Release();
    }

    // Set task to run with highest privileges (admin) for the logged-in user
    IPrincipal* pPrincipal = NULL;
    hr = pTask->get_Principal(&pPrincipal);
    if (SUCCEEDED(hr))
    {
        // Set the user ID to the actual logged-in user (not the elevated admin)
        if (!loggedInUser.IsEmpty())
        {
            CStringW userW(loggedInUser);
            pPrincipal->put_UserId(_bstr_t(userW));
        }
        
        pPrincipal->put_LogonType(TASK_LOGON_INTERACTIVE_TOKEN);
        pPrincipal->put_RunLevel(TASK_RUNLEVEL_HIGHEST);
        pPrincipal->Release();
    }

    // Set task settings
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (SUCCEEDED(hr))
    {
        pSettings->put_StartWhenAvailable(VARIANT_TRUE);
        pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
        pSettings->put_StopIfGoingOnBatteries(VARIANT_FALSE);
        pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT0S")); // No time limit
        pSettings->Release();
    }

    // Add logon trigger for the specific user with a delay to let Explorer load
    ITriggerCollection* pTriggers = NULL;
    hr = pTask->get_Triggers(&pTriggers);
    if (SUCCEEDED(hr))
    {
        ITrigger* pTrigger = NULL;
        hr = pTriggers->Create(TASK_TRIGGER_LOGON, &pTrigger);
        if (SUCCEEDED(hr))
        {
            pTrigger->put_Id(_bstr_t(L"LogonTrigger"));
            pTrigger->put_Enabled(VARIANT_TRUE);
            
            // Set the trigger to fire only for the logged-in user
            ILogonTrigger* pLogonTrigger = NULL;
            if (SUCCEEDED(pTrigger->QueryInterface(IID_ILogonTrigger, (void**)&pLogonTrigger)))
            {
                if (!loggedInUser.IsEmpty())
                {
                    CStringW userW(loggedInUser);
                    pLogonTrigger->put_UserId(_bstr_t(userW));
                }
                
                // Add 30 second delay to ensure Explorer/shell is fully loaded
                // This prevents the tray icon from failing to appear
                pLogonTrigger->put_Delay(_bstr_t(L"PT30S"));
                
                pLogonTrigger->Release();
            }
            
            pTrigger->Release();
        }
        pTriggers->Release();
    }

    // Add action (run the exe with -minimize)
    IActionCollection* pActions = NULL;
    hr = pTask->get_Actions(&pActions);
    if (SUCCEEDED(hr))
    {
        IAction* pAction = NULL;
        hr = pActions->Create(TASK_ACTION_EXEC, &pAction);
        if (SUCCEEDED(hr))
        {
            IExecAction* pExecAction = NULL;
            hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
            if (SUCCEEDED(hr))
            {
                // Convert CString to BSTR
                CStringW exePathW(exePath);
                pExecAction->put_Path(_bstr_t(exePathW));
                pExecAction->put_Arguments(_bstr_t(L"-minimize"));
                
                // Set working directory to exe folder
                CString workDir = exePath;
                int lastSlash = workDir.ReverseFind(_T('\\'));
                if (lastSlash > 0)
                {
                    workDir = workDir.Left(lastSlash);
                    CStringW workDirW(workDir);
                    pExecAction->put_WorkingDirectory(_bstr_t(workDirW));
                }
                
                pExecAction->Release();
            }
            pAction->Release();
        }
        pActions->Release();
    }

    // Register the task
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"Toolfish"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(),  // Current user
        _variant_t(),
        TASK_LOGON_INTERACTIVE_TOKEN,
        _variant_t(L""),
        &pRegisteredTask);

    if (SUCCEEDED(hr))
    {
        log_msg("Admin startup task created successfully in Task Scheduler.");
        pRegisteredTask->Release();
    }
    else
    {
        log_error("CreateAdminStartupTask: Failed to register task (HRESULT: 0x%08X)", hr);
        // Show error to user - this is important feedback
        CString errMsg;
        errMsg.Format(_T("Failed to create admin startup task.\n\nError code: 0x%08X\n\nMake sure Toolfish is running as Administrator."), hr);
        MessageBox(NULL, errMsg, _T("Toolfish - Task Scheduler Error"), MB_OK | MB_ICONERROR);
    }

    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    return SUCCEEDED(hr);
}

// Helper to delete registry Run entry
// When elevated, we need to access the logged-in user's registry via HKEY_USERS\<SID>
static void DeleteRegistryStartup()
{
    // If running elevated, we need to use HKEY_USERS\<SID> to access the actual user's registry
    if (IsUserAnAdmin())
    {
        CString userSID = GetLoggedInUserSID();
        if (!userSID.IsEmpty())
        {
            // Build path: HKEY_USERS\<SID>\Software\Microsoft\Windows\CurrentVersion\Run
            CString fullPath;
            fullPath.Format(_T("%s\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), (LPCTSTR)userSID);
            
            HKEY hKey;
            if (RegOpenKeyEx(HKEY_USERS, fullPath, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
            {
                RegDeleteValue(hKey, app_glo.GetAppName());
                RegCloseKey(hKey);
            }
        }
    }
    else
    {
        // Not elevated - HKEY_CURRENT_USER works fine
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER, 
            _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
        {
            RegDeleteValue(hKey, app_glo.GetAppName());
            RegCloseKey(hKey);
        }
    }
}

// Helper to create registry Run entry
static void CreateRegistryStartup(const CString& exePath)
{
    CRegistry reg;
    CString sRegPath("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    
    reg.CreateKey(HKEY_CURRENT_USER, sRegPath);
    
    CString cst_regdata = exePath;
    cst_regdata += _T(" -minimize");
    
    reg.WriteString(app_glo.GetAppName(), cst_regdata);
}

// Check if registry startup entry exists
static bool HasRegistryStartup()
{
    CRegistry reg;
    CString sRegPath("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    
    if (!reg.VerifyKey(HKEY_CURRENT_USER, sRegPath))
        return false;
    
    return reg.VerifyValue(app_glo.GetAppName());
}

bool global_registry( bool return_only, bool add, bool as_admin )
{
    // Get exe path
    TCHAR st_full_path[256];
    GetModuleFileName(NULL, st_full_path, 255);
    CString exePath = st_full_path;
    exePath.Replace(_T("\""), _T(""));

    // Just checking?
    if (return_only)
    {
        return HasRegistryStartup();
    }

    // CASE 1: Admin startup enabled
    if (as_admin && add)
    {
        // Delete registry entry (we don't want both)
        DeleteRegistryStartup();
        
        // Create Task Scheduler task
        return CreateAdminStartupTask(exePath);
    }
    
    // CASE 2: Admin startup being disabled
    if (as_admin && !add)
    {
        // Delete the task
        DeleteAdminStartupTask();
        return true;
    }
    
    // CASE 3: Normal (non-admin) startup
    // Always clean up the task first (user might be switching from admin to normal)
    DeleteAdminStartupTask();
    
    if (add)
    {
        // Create registry entry
        CreateRegistryStartup(exePath);
    }
    else
    {
        // Delete registry entry
        DeleteRegistryStartup();
    }
    
    return true;
}

