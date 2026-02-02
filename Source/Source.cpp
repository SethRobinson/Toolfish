// Source.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include "Source.h"
#include "CDlgMain.h"
#include "CGlobals.h"
#include "registry_util.h"
#include "..\Shared\RichEditCtrlEx.h"
#include "CDlgNag.h"
#include "..\Shared\CProtection.h"
#include "..\Shared\std_all.h"
#include "demo_util.h"
#include "DlgWelcome.h"       
#include "CDlgIntro.h"
//#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "shared_funcs.h"


/////////////////////////////////////////////////////////////////////////////
// CSource

BEGIN_MESSAGE_MAP(CSource, CWinApp)
	//{{AFX_MSG_MAP(CSource)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSource construction

CSource::CSource()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSource object

CSource theApp;

/////////////////////////////////////////////////////////////////////////////
// CSource initialization

BOOL CSource::InitInstance()
{
	randomize();

    AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.



#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
   AfxInitRichEditEx( );
   _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
   
   	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("Unable to init Winsock. Are you trying to run this on Windows 3.1 or something?"));
		return FALSE;
	}

    if (CoInitialize(NULL) == S_FALSE)
    {
        //COM library may have already been initialized in this thread, happens when using some debuggers..
        //assert(0); //big time error
    }

    // Check for -waitpid: parameter (used when restarting elevated)
    // This makes us wait for the old instance to exit before continuing
    {
        CString strCommand(GetCommandLine());
        int waitPidPos = strCommand.Find(_T("-waitpid:"));
        if (waitPidPos != -1)
        {
            // Extract the PID
            CString pidStr = strCommand.Mid(waitPidPos + 9);
            int spacePos = pidStr.Find(_T(' '));
            if (spacePos != -1)
                pidStr = pidStr.Left(spacePos);
            
            DWORD pid = (DWORD)_ttoi(pidStr);
            if (pid != 0)
            {
                // Wait for the old process to exit (up to 10 seconds)
                HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid);
                if (hProcess != NULL)
                {
                    WaitForSingleObject(hProcess, 10000);
                    CloseHandle(hProcess);
                }
                // Small additional delay to ensure cleanup
                Sleep(500);
            }
        }
    }

#ifndef _DEBUG
    
    //check for another instance of this running
   CreateMutex(NULL,TRUE,_T("Toolfish")); // mutex will be automatically deleted when process ends. 
   BOOL bAlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS); 
   if(bAlreadyRunning) 
   {
       //uh oh
       MessageBox(NULL, _T("An instance of Toolfish is already running."),app_glo.GetAppName(), NULL);
       return FALSE; 
   }
  
#endif   
   
   switch_to_current_dir();

   //if the file "notes.txt" doesn't exist let's create it with some default
    //crap so our notes open hotkey works

    if (!exist(_T("notes.txt")))
    {
        //create it
        add_text("** NOTES FILE **\r\n\r\n", "notes.txt");
        add_text("Use this text file to scribble notes or ideas, if you don't already have something like this it's very useful.\r\n", "notes.txt");
        
    }


   /*
      int i_days = GetDaysSinceDate(10, 10,2002);

        if ( (i_days > 30) || (i_days < 0))
        {
              MessageBox(NULL,_T("This beta version has expired."), _T("Toolfish Ain't In Beta No More!"), MB_OK | MB_ICONSTOP);
             return false;
   
        }
     */

   
    	//first let's load out defaults
	FileConfigLoad(&glo);
    //if we loaded a newer version then we are, say something because we're gonna
    //corrupt everything
    if (glo.f_version > C_F_VERSION)
    {
        int result = MessageBox(NULL, _T("Data from a newer version of Toolfish has been detected.\n\nWould you like to continue and use this older version of Toolfish with it anyway? (may corrupt data)"), _T("Data for newer version detected"), MB_OKCANCEL );

      if (result == IDCANCEL )
      {
         
          return 0;
      }
    }
    //Process load at boot stuff
    //If using admin mode (Task Scheduler), only update if we're actually running as admin
    //Otherwise we'd delete the task and not be able to recreate it
    if (glo.m_b_boot_admin && glo.m_b_boot_load)
    {
        // Using Task Scheduler - only update if running elevated
        if (IsUserAnAdmin())
        {
            // Remove old registry entry if present, then create/update task
            global_registry(false, false, false);  // Clean up registry
            global_registry(false, true, true);    // Create task
        }
        // If not admin, leave the existing task alone
    }
    else
    {
        // Using normal registry method - safe to update anytime
        global_registry(false, false, false);  // Remove old entry (in case dir moved)
        global_registry(false, glo.m_b_boot_load, false);  // Add registry entry if enabled
    }
   
     
     //first things first.  Copy protection.

     //if a demo, show this:
     cls_pro.UseFixedReduce(true); //use newer methods
     cls_pro.SetSerialMod(7.4f, 27362);

#ifdef _USES_PROTECTION
     

     if  (   (glo.m_st_code[0] == 0) ||
         (glo.m_st_unlock_key[0] == 0)
         )
         
     {
         InitTrialStuff();   
         //no has been entered, run nag screen.
         CDlgNag dlg_nag;
         if (dlg_nag.DoModal() == IDCANCEL)
         {
             return 0;
         }
         if  (   (glo.m_st_code[0] == 0) ||
             (glo.m_st_unlock_key[0] == 0)
             )
         {
             //we're still not registered but somehow they clicked ok while the trial period
             //was over?  I don't think so.
             
             if (app_glo.GetTrialDaysLeft() <= 0)
             {
                 return 0;
             }
         }

     } 
  
     
      if  (   (glo.m_st_code[0] != 0) ||
         (glo.m_st_unlock_key[0] != 0)
         )
      {
     
         //this is supposedly the registered version.
        
        app_glo.SetTrialDaysLeft(-1); //this indicates it's a registered version
         cls_pro.demake(glo.m_st_code, glo.m_st_unlock_key);
         
         
         if (!cls_pro.is_validated())
         {
             //code was bad.  let's do an easy to hack message.  (don't worry, the tricky security comes later)
             
             glo.m_st_code[0] = 0;
             glo.m_st_unlock_key[0] = 0;
             FileConfigSave(&glo);
 
             MessageBox(NULL,_T("The unlock key is not valid.  Please restart program to enter it again."), _T("Error"), MB_OK | MB_ICONSTOP);
             return false;
         }
         
       
     }
#endif      
        //if first time, show this
        if (!glo.m_b_showed_welcome)
        {
            
            CDlgWelcome dlg;
            dlg.DoModal();
            CDlgIntro dlg2;
            dlg2.DoModal();
            glo.m_b_showed_welcome = true;
            FileConfigSave(&glo);
        }


   //setup inital mute timer settings  (they are taken from Glo)
   app_glo.ResetMuteTimer();
 
   	m_pMainWnd = new CDlgMain;

   if (m_pMainWnd)
      return TRUE;
   else
      return FALSE;

}

void CSource::WinHelp( DWORD dwData, UINT nCmd )
{
   
LogMsg(_T("Launching Help."));
int result = (int)ShellExecute(NULL,_T("open"),_T("docs\\help.htm"), NULL,NULL, SW_MAXIMIZE);
ProcessError(m_pMainWnd->m_hWnd, result, _T("temp.url"),_T("docs\\help.htm")); //show an error message if we couldn't open this

}



