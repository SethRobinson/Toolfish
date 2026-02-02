#include "reboot_util.h"
#include "CEvent.h"

bool ProcessReboot(CEvent *p_event, int i_index)
{
   
    CAction *p_action = p_event->GetAction(i_index);

    if (p_action->GetRebootType() == C_ACTION_REBOOT_LOGOFF)
    {
        LogMsg(_T("Logging off user..."));
        InitiateOSShutdown(EWX_LOGOFF);
    }

    if (p_action->GetRebootType() == C_ACTION_REBOOT_SHUTDOWN)
    {
                LogMsg(_T("Shutting down computer..."));
                InitiateOSShutdown(EWX_SHUTDOWN);
    }


    if (p_action->GetRebootType() == C_ACTION_REBOOT_RESTART)
    {
        LogMsg(_T("Rebooting computer..."));
        InitiateOSShutdown(EWX_REBOOT);
    }



 return true; //success
}

void InitiateOSShutdown(UINT uTheRequestType)
{
	BOOL bRetVal=FALSE ;

	#ifdef _UNICODE
		HANDLE hToken=0 ;
		TOKEN_PRIVILEGES tkp ;

		// Get a token for this process. 
 		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
        {
            LogError(_T("Error opening process token for logoff/shutdown/reboot command."));
            //return;
        }

		// Get the LUID for the shutdown privilege. 
 		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid) ;

		tkp.PrivilegeCount = 1;  // one privilege to set
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED ;

		// Get the shutdown privilege for this process.
 		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0) ;

		// Cannot test the return value of AdjustTokenPrivileges.
 		if (GetLastError() != ERROR_SUCCESS)
        {
            LogError(_T("Error opening process token for logoff/shutdown/reboot command."));
        }
	#endif	// _WIN_NT

	// Shutdown The System And Force All Applications To Close ...
	bRetVal = ExitWindowsEx(EWX_FORCE|uTheRequestType, 0) ;
	assert(bRetVal == TRUE) ;

	return ;
}
