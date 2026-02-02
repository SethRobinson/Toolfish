// CDlgOptions.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgOptions.h"
#include "CGlobals.h"
#include "registry_util.h"
#include "CDlgVoice.h"  
#include "CVoice.h"     
#include "CDlgOkCancel.h"
#include "CSMPTSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions dialog


CDlgOptions::CDlgOptions(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptions)
	m_b_boot_load = FALSE;
	m_i_log_lines = 0;
	m_b_trim_log = FALSE;
	m_b_disable_tts = FALSE;
	m_b_boot_stealth = FALSE;
	m_b_boot_admin = FALSE;
	m_b_enable_tray = FALSE;
	m_b_enable_blink_email = FALSE;
	m_b_enable_blink_webpage = FALSE;
	m_b_detect_IP_address_server_side = FALSE;
	m_b_minimize_on_close = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptions)
	DDX_Check(pDX, IDC_BOOT_LOAD, m_b_boot_load);
	DDX_Text(pDX, IDC_LOG_LINES, m_i_log_lines);
	DDX_Check(pDX, IDC_TRIM_LOG, m_b_trim_log);
	DDX_Check(pDX, IDC_B_DISABLE_TTS, m_b_disable_tts);
	DDX_Check(pDX, IDC_BOOT_STEALTH, m_b_boot_stealth);
	DDX_Check(pDX, IDC_BOOT_ADMIN, m_b_boot_admin);
	DDX_Check(pDX, IDC_B_ENABLE_TRAY, m_b_enable_tray);
	DDX_Check(pDX, IDC_BLINK_EMAIL, m_b_enable_blink_email);
	DDX_Check(pDX, IDC_BLINK_WEBPAGE, m_b_enable_blink_webpage);
	DDX_Check(pDX, IDC_B_DETECT_IP_SERVER_SIDE, m_b_detect_IP_address_server_side);
	DDX_Check(pDX, IDC_B_MINIMIZE_ON_CLOSE, m_b_minimize_on_close);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptions, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgOptions)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TRIM_LOG, OnTrimLog)
	ON_BN_CLICKED(IDC_VOICE_CONFIGURE, OnVoiceConfigure)
	ON_BN_CLICKED(IDC_B_DISABLE_TTS, OnBDisableTts)
	ON_BN_CLICKED(IDC_BOOT_LOAD, OnBootLoad)
	ON_BN_CLICKED(IDC_BOOT_STEALTH, OnBootStealth)
	ON_BN_CLICKED(IDC_BOOT_ADMIN, OnBootAdmin)
	ON_BN_CLICKED(IDC_SMTP_CONFIGURE, OnSmtpConfigure)
	ON_BN_CLICKED(IDC_B_ENABLE_TRAY, OnBEnableTray)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions message handlers

BOOL CDlgOptions::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	SetBitmap(_T("gui\\background.bmp"));

	m_b_boot_load = glo.m_b_boot_load;
    m_b_trim_log = glo.m_b_trim_log;
    m_i_log_lines = glo.m_i_trim_lines;
    m_b_disable_tts = glo.m_b_disable_tts;
    m_b_boot_stealth = glo.m_b_boot_stealth;
    m_b_boot_admin = glo.m_b_boot_admin;
    m_b_enable_tray = !glo.m_b_tray_disable_blinking;
    m_b_enable_blink_webpage = !glo.m_b_tray_no_trigger_on_webcompare;
    m_b_enable_blink_email = !glo.m_b_tray_no_trigger_on_email;
    m_b_detect_IP_address_server_side = !glo.m_b_server_ip_disabled;
    m_b_minimize_on_close = glo.m_b_minimize_on_close;

    // Show current elevation status
    if (IsUserAnAdmin())
    {
        SetDlgItemText(IDC_ADMIN_STATUS, _T("(Running as Admin)"));
    }
    else
    {
        SetDlgItemText(IDC_ADMIN_STATUS, _T("(Not Admin)"));
    }

    UpdateData(D_TO_WINDOW);
    ProcessGray();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDlgOptions::OnOK() 
{
   UpdateData(D_TO_VAR);
   glo.m_b_boot_load = m_b_boot_load != 0;
   glo.m_b_trim_log = m_b_trim_log != 0;
   glo.m_i_trim_lines = m_i_log_lines;
   glo.m_b_disable_tts = m_b_disable_tts != 0;
   glo.m_b_boot_stealth = m_b_boot_stealth != 0;
   glo.m_b_boot_admin = m_b_boot_admin != 0;
   
   glo.m_b_tray_disable_blinking = m_b_enable_tray != 1; //reversed on purpose
   glo.m_b_tray_no_trigger_on_webcompare = m_b_enable_blink_webpage != 1; //reversed on purpose
   glo.m_b_tray_no_trigger_on_email =  m_b_enable_blink_email != 1; //reversed
   glo.m_b_server_ip_disabled =  m_b_detect_IP_address_server_side != 1; //reversed
   glo.m_b_minimize_on_close = m_b_minimize_on_close != 0;
   
   ProcessGray();

   // Handle startup registration - 3 cases:
   // 1. Admin startup enabled (boot_load && boot_admin) - need task, no registry
   // 2. Normal startup enabled (boot_load && !boot_admin) - need registry, no task
   // 3. Startup disabled (!boot_load) - no task, no registry
   
   if (glo.m_b_boot_load && glo.m_b_boot_admin)
   {
       // Check if the logged-in user is actually an admin
       // (Task Scheduler can only auto-elevate users who are already admins)
       if (!IsLoggedInUserAdmin())
       {
           CString userName = GetLoggedInUserName();
           CString msg;
           msg.Format(
               _T("The current user '%s' is not a member of the Administrators group.\n\n")
               _T("Windows Task Scheduler can only auto-elevate users who are already administrators. ")
               _T("For non-admin users, it will create the startup task but Toolfish will NOT have admin privileges.\n\n")
               _T("To fix this, add '%s' to the Administrators group:\n")
               _T("1. Open Computer Management (run 'compmgmt.msc')\n")
               _T("2. Go to Local Users and Groups > Groups > Administrators\n")
               _T("3. Add '%s' to this group\n")
               _T("4. Log out and back in\n\n")
               _T("Do you want to continue anyway? (Toolfish will start at login but without admin privileges)"),
               (LPCTSTR)userName, (LPCTSTR)userName, (LPCTSTR)userName);
           
           if (MessageBox(msg, _T("User Not Administrator"), MB_YESNO | MB_ICONWARNING) != IDYES)
           {
               // User cancelled - revert setting
               glo.m_b_boot_admin = false;
               m_b_boot_admin = FALSE;
               UpdateData(D_TO_WINDOW);
               FileConfigSave(&glo);
               CBkDialogST::OnOK();
               return;
           }
       }
       
       // CASE 1: Admin startup - need elevation to create task
       if (IsUserAnAdmin())
       {
           // Deletes registry, creates task
           global_registry(false, true, true);
       }
       else
       {
           // Need to restart elevated
           if (MessageBox(
               _T("Creating an admin startup task requires administrator privileges.\n\n")
               _T("Would you like to restart Toolfish as administrator now?"),
               _T("Elevation Required"),
               MB_YESNO | MB_ICONQUESTION) == IDYES)
           {
               FileConfigSave(&glo);
               
               TCHAR exePath[MAX_PATH];
               GetModuleFileName(NULL, exePath, MAX_PATH);
               
               CString params;
               params.Format(_T("-show -waitpid:%d"), GetCurrentProcessId());
               
               AllowSetForegroundWindow(ASFW_ANY);
               
               SHELLEXECUTEINFO sei = { sizeof(sei) };
               sei.fMask = SEE_MASK_NOCLOSEPROCESS;
               sei.lpVerb = _T("runas");
               sei.lpFile = exePath;
               sei.lpParameters = params;
               sei.nShow = SW_SHOWNORMAL;
               
               if (ShellExecuteEx(&sei))
               {
                   ExitProcess(0);
               }
           }
           // User declined - revert setting
           glo.m_b_boot_admin = false;
       }
   }
   else if (glo.m_b_boot_load && !glo.m_b_boot_admin)
   {
       // CASE 2: Normal startup - deletes task, creates registry
       global_registry(false, true, false);
   }
   else
   {
       // CASE 3: Startup disabled - deletes both task and registry
       global_registry(false, false, true);   // Delete task
       global_registry(false, false, false);  // Delete registry
   }
   
   FileConfigSave(&glo);

    
    app_glo.ActivateSpeech(!glo.m_b_disable_tts);

    app_glo.RetrieveIPAddress(); //in case they changed this option..

	CBkDialogST::OnOK();
}

void CDlgOptions::ProcessGray()
{
  UpdateData(D_TO_VAR);
  
  ((CWnd*)GetDlgItem(IDC_LOG_LINES))->EnableWindow(m_b_trim_log);
  ((CWnd*)GetDlgItem(IDC_BOOT_STEALTH))->EnableWindow(m_b_boot_load);
  ((CWnd*)GetDlgItem(IDC_BOOT_ADMIN))->EnableWindow(m_b_boot_load);
  ((CWnd*)GetDlgItem(IDC_ADMIN_EXPLANATION))->EnableWindow(m_b_boot_load);
     
  ((CWnd*)GetDlgItem(IDC_BLINK_WEBPAGE))->EnableWindow(m_b_enable_tray);
  ((CWnd*)GetDlgItem(IDC_BLINK_EMAIL))->EnableWindow(m_b_enable_tray);
}


HBRUSH CDlgOptions::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
	
switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
        if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
              pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }

        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
            ||            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
 
            )
        {
            // log_msg("Group box detected.");
            pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) m_hbrush;
            return hbr;
        }
        
        //default 
        
        //pDC->SetTextColor(MY_TEXTCOLOR);
        pDC->SetBkMode(TRANSPARENT);
        hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
        return hbr;
        break;
      
    }
	return hbr;
}

void CDlgOptions::OnTrimLog() 
{
	// TODO: Add your control notification handler code here
    UpdateData(D_TO_VAR);
    ProcessGray();	
}

void CDlgOptions::OnVoiceConfigure() 
{
     
   CDlgVoice dlg;
   dlg.SetPointerToVoiceClass(&glo.m_voice);
   dlg.DoModal();
       
}

void CDlgOptions::OnBDisableTts() 
{
    UpdateData(D_TO_VAR);

 //  app_glo.ActivateSpeech(!m_b_disable_tts);


    // TODO: Add your control notification handler code here
	
}

void CDlgOptions::OnBootLoad() 
{
	// TODO: Add your control notification handler code here
ProcessGray();	
}

void CDlgOptions::OnBootStealth() 
{
    UpdateData(D_TO_VAR);

    if (m_b_boot_stealth)
    {
        
        
        DlgOkCancel dlg;
        dlg.SetTitle(_T("WARNING: Stealth mode hides the Toolfish tray icon completely!"));
        dlg.SetDialogText(_T("Once in stealth mode the ONLY way to open Toolfish is to use the Stealth Toggle hotkey.\r\n\r\nThis is Ctrl-Shift-Alt-F12 by default.\r\n\r\nAre you sure you want Toolfish to boot into stealth mode?"));
        if (dlg.DoModal() == IDOK)
        {
            
        } else
        {
            m_b_boot_stealth = false;
         UpdateData(D_TO_WINDOW);


        }
    }
}

void CDlgOptions::OnBootAdmin() 
{
    UpdateData(D_TO_VAR);

    if (m_b_boot_admin)
    {
        // Check if we're already running as admin
        if (!IsUserAnAdmin())
        {
            // Need elevation to create the scheduled task
            if (MessageBox(
                _T("Running as Admin at startup requires creating a scheduled task with administrator privileges.\n\n")
                _T("This allows Toolfish to work with games and apps that run elevated (like Genshin Impact).\n\n")
                _T("Would you like to restart Toolfish as administrator now to enable this?"),
                _T("Elevation Required"),
                MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                // Save current settings first
                glo.m_b_boot_load = m_b_boot_load != 0;
                glo.m_b_boot_admin = TRUE;  // Set this to true so it's remembered
                FileConfigSave(&glo);

                // Get current exe path and restart elevated
                TCHAR exePath[MAX_PATH];
                GetModuleFileName(NULL, exePath, MAX_PATH);
                
                // Use -waitpid to tell the new instance to wait for us to exit
                // Use -show to force window to foreground so user sees the restart
                CString params;
                params.Format(_T("-show -waitpid:%d"), GetCurrentProcessId());
                
                // Allow the new process to take foreground (otherwise Windows blocks it)
                AllowSetForegroundWindow(ASFW_ANY);
                
                SHELLEXECUTEINFO sei = { sizeof(sei) };
                sei.fMask = SEE_MASK_NOCLOSEPROCESS;  // Get process handle
                sei.lpVerb = _T("runas");
                sei.lpFile = exePath;
                sei.lpParameters = params;
                sei.nShow = SW_SHOWNORMAL;
                
                if (ShellExecuteEx(&sei))
                {
                    // Exit current instance immediately
                    ExitProcess(0);
                }
                else
                {
                    // User cancelled UAC or it failed
                    glo.m_b_boot_admin = FALSE;
                    FileConfigSave(&glo);
                    m_b_boot_admin = FALSE;
                    UpdateData(D_TO_WINDOW);
                    MessageBox(
                        _T("Failed to restart as administrator. The admin startup option has been disabled."),
                        _T("Error"),
                        MB_OK | MB_ICONWARNING);
                }
            }
            else
            {
                // User declined, uncheck the box
                m_b_boot_admin = FALSE;
                UpdateData(D_TO_WINDOW);
            }
        }
        // If already admin, the task will be created when OK is clicked
    }
    ProcessGray();
}

void CDlgOptions::OnSmtpConfigure() 
{
    CSMPTSetup dlg;
    dlg.DoModal();

}

void CDlgOptions::OnBEnableTray() 
{
    ProcessGray();
}

