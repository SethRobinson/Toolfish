#include "stdafx.h"
#include "registry_util.h"
#include "CGlobals.h"
#include "..\Shared\registry.h"
#include <taskschd.h>
#include <comdef.h>
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

// Create a scheduled task that runs Toolfish at logon with highest privileges (admin)
bool CreateAdminStartupTask(const CString& exePath)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE)
    {
        log_error("CreateAdminStartupTask: CoInitialize failed");
        return false;
    }

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

    // Set task to run with highest privileges (admin)
    IPrincipal* pPrincipal = NULL;
    hr = pTask->get_Principal(&pPrincipal);
    if (SUCCEEDED(hr))
    {
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

    // Add logon trigger
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

bool global_registry( bool return_only, bool add, bool as_admin )
{
    // Get exe path for use in either method
    TCHAR st_full_path[256];
    GetModuleFileName(NULL, st_full_path, 255);
    CString exePath = st_full_path;
    exePath.Replace(_T("\""), _T(""));

    CRegistry reg;
    CString sRegPath("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

    if (!reg.VerifyKey(HKEY_CURRENT_USER, sRegPath))
    {
        log_error("Error - Run reg key doesn't exist!");
        return false;
    }

    if (as_admin && add)
    {
        // Using Task Scheduler for admin startup
        // First remove any registry entry to avoid double-launch
        if (reg.VerifyValue(app_glo.GetAppName()))
        {
            reg.DeleteValue(app_glo.GetAppName());
        }
        
        // Create the scheduled task
        return CreateAdminStartupTask(exePath);
    }
    else if (as_admin && !add)
    {
        // Removing admin startup - delete the task
        DeleteAdminStartupTask();
        return true;
    }
    else
    {
        // Normal (non-admin) startup using registry
        // If we're adding a registry entry and running as admin, clean up any old task
        // (This handles switching from admin startup to normal startup)
        if (add && IsUserAnAdmin())
        {
            DeleteAdminStartupTask();
        }

        if (!reg.VerifyValue(app_glo.GetAppName()))
        {
            // Key does not exist
            if (!add)
            {
                // We were here to delete, and we don't have to. Get out!
                return false;
            }
            
            if (return_only)
            {
                return false;
            }
            
            reg.CreateKey(HKEY_CURRENT_USER, sRegPath);

            CString cst_regdata = exePath;
            cst_regdata += _T(" -minimize");
            
            reg.WriteString(app_glo.GetAppName(), cst_regdata);
        }
        else
        {
            // Key exists
            if (return_only)
            {
                return true;
            }

            if (!add)
            {
                // Delete the key
                reg.DeleteValue(app_glo.GetAppName());
            }
        }
    }
    
    return true;
}

