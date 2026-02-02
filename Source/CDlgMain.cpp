// CDlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgMain.h"
#include "CGlobals.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\std_all.h"
#include "..\Shared\CSpeech.h"
#include "..\Shared\CTextParse.h"
#include "..\Shared\Uni.h"
#include "CDlgKeyLog.h"
#include "CDlgOptions.h"
#include "CKeyLog.h"
#include "CEventList.h"
#include "CDlgAbout.h"
#include "CDlgMute.h"
#include "mute_util.h"
#include "gamepad_util.h"
#include "spy_util.h"        
#include "..\Shared\winsock\winsock_util.h"
#include "CDlgWizard.h"
#include "DlgWelcome.h"
#include "CDlgOkCancel.h"
#include "CDlgThread.h"
#include "file_config.h"
#include "CDlgQuit.h"
#include "DlgWizardOTReminder.h"
#include "CDlgWizardRecurrentReminder.h"
#include "CDlgWizardAtomic.h"
#include "CDlgAlert.h"
#include "CDlgWizardEmail.h"
#include "CDlgMailView.h"
#include "CDlgWizardMonitor.h"
#include "CDlgFun.h"
#include "Maint.h" 
#include "DlgWizardReminderX.h"

bool g_b_tray_active = false;

bool g_b_minimized = false;

void ColorizeLine(CRichEditCtrlEx * p_rich_edit, int i_line_to_color);
const int C_TRAY_BLINK_SPEED = 180; //in ms
const int C_TRAY_ICON_NORMAL = 0;
const int C_TRAY_ICON_ALARM = 1;
const int C_TRAY_MODE_NORMAL = 0;
const int C_TRAY_MODE_BLINK = 1;

const int C_MAIN_TIMER_MSECS = 400;
const int C_GAMEPAD_TIMER_MSECS = 50;  // Fast polling for responsive gamepad detection
const int C_GAMEPAD_TIMER_ID = 2;

const int C_MAIN_TIMER_MAINT = 1000*59; //almost one minute
const int C_MAIN_TIMER_AUTO_SAVE = 1000*60*30; //save our globals every 30 minutes, so we don't lose mouse clicks
//in the case of a crash

const int C_MAIN_TIMER_IP_CHECK = 1000*60*60*6; //check ip every 6 hours


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgEvents.h"
#include "ProcessEvents.h"
BOOL SetForegroundWindowEx(HWND hWnd);

HMODULE dll_hinstance = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMain)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
 
    Create (IDD, pParent); // Create modeless dialog. 
  
      CString str_command(GetCommandLine());
	 if (str_command.Find(_T("-minimize")) == -1)
	 {
       g_b_minimized = false;
         //startup fullscreen
      ShowWindow(SW_SHOW);
     }  else
     {
       g_b_minimized = true;

     }
  
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMain)
	DDX_Control(pDX, IDC_FUN, m_but_fun);
	DDX_Control(pDX, IDC_EMAILVIEW, m_but_email);
	DDX_Control(pDX, IDC_OK, m_but_hide);
	DDX_Control(pDX, IDC_CLEAR, m_but_clear);
	DDX_Control(pDX, IDC_BUT_HELP, m_but_help);
	DDX_Control(pDX, IDC_LOGO, m_but_logo);
	DDX_Control(pDX, IDC_OPTIONS, m_but_options);
	DDX_Control(pDX, IDC_SUGGEST, m_but_suggestion);
	DDX_Control(pDX, IDC_WIZARD, m_but_wizard);
	DDX_Control(pDX, IDC_KEY_LOG, m_but_key_log);
	DDX_Control(pDX, IDC_MUTE, m_but_mute);
	DDX_Control(pDX, IDC_LOG, m_cedit_log);
    DDX_Control(pDX, IDC_EVENTS, m_but_events);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgMain, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgMain)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EVENTS, OnEvents)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_KEY_LOG, OnKeyLog)
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_MUTE, OnMute)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_COMMAND(ID_MENU_OPTIONS, OnMenuOptions)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_WIZARD, OnWizard)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_LOGO, OnLogo)
	ON_BN_CLICKED(IDC_BUT_HELP, OnButHelp)
	ON_COMMAND(ID_MENU_WIZARD_WEB, OnMenuWizardWeb)
	ON_COMMAND(ID_MENU_WIZARD_ONE_TIME_REMINDER, OnMenuWizardOneTimeReminder)
	ON_COMMAND(ID_MENU_VOICE_MUTE, OnMenuVoiceMute)
	ON_COMMAND(ID_ABOUT_TOOLFISH, OnAboutToolfish)
	ON_COMMAND(ID_DISABLE_SMARTMUTE, OnDisableSmartmute)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER, OnMenuWizardReminder)
	ON_COMMAND(ID_MENU_WIZARD_ATOMIC, OnMenuWizardAtomic)
	ON_COMMAND(ID_MENU_READ_CLIPBOARD, OnMenuReadClipboard)
	ON_COMMAND(ID_MENU_WIZARD_EMAIL, OnMenuWizardEmail)
	ON_BN_CLICKED(IDC_EMAILVIEW, OnEmailview)
	ON_COMMAND(ID_MENU_WIZARD_MONITOR, OnMenuWizardMonitor)
	ON_BN_CLICKED(IDC_FUN, OnFun)
    ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_1_DAY, OnMenuWizardReminder1Day)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_1_HOUR, OnMenuWizardReminder1Hour)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_1_MONTH, OnMenuWizardReminder1Month)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_1_WEEK, OnMenuWizardReminder1Week)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_10, OnMenuWizardReminder10)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_15, OnMenuWizardReminder15)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_2_HOUR, OnMenuWizardReminder2Hour)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_30, OnMenuWizardReminder30)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_4_HOURS, OnMenuWizardReminder4Hours)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_5, OnMenuWizardReminder5)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_8_HOURS, OnMenuWizardReminder8Hours)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_45, OnMenuWizardReminder45)
	ON_COMMAND(ID_MENU_WIZARD_REMINDER_20, OnMenuWizardReminder20)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMain message handlers

                                      
void * WINAPI ExeMalloc(long l_size)
{
    return malloc(l_size);
}
                  
                 

BOOL CDlgMain::OnInitDialog()
{
	CBkDialogST::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog

   m_timer_ip_check.set_interval(C_MAIN_TIMER_IP_CHECK); //every six hours
    m_timer_auto_save.set_interval(C_MAIN_TIMER_AUTO_SAVE);
   m_timer_maint.set_interval(C_MAIN_TIMER_MAINT);
   ActivateEasyMoveMode(TRUE);
   
  m_hIcon_small = (HICON) ::LoadImage(AfxGetApp()->m_hInstance,
  MAKEINTRESOURCE(IDR_MAINFRAME),
  IMAGE_ICON, 16, 16,
  LR_DEFAULTCOLOR);

  
  m_hIcon_small_alarm = (HICON) ::LoadImage(AfxGetApp()->m_hInstance,
  MAKEINTRESOURCE(IDR_FISH_ALARM),
  IMAGE_ICON, 16, 16,
  LR_DEFAULTCOLOR);
  
  m_timer_tray.set_interval(C_TRAY_BLINK_SPEED);
   TrayStopBlink(0); //the parm isn't actually used yet

    SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon_small, FALSE);		// Set small icon
     
    // TODO: Add extra initialization here
    app_glo.SetHWND(this->m_hWnd);
    app_glo.SetDialogHWND(this->m_hWnd);
    
    SetBitmap(_T("gui\\background.bmp"));
    
    m_cedit_log.AutoURLDetect(true);
    m_cedit_log.SetEventMask(m_cedit_log.GetEventMask () | ENM_LINK);
  
  
   //let's init the update ticker
    app_glo.m_timer_ident = SetTimer(1, C_MAIN_TIMER_MSECS, NULL );   //accuracy of overall timing
    if (!app_glo.m_timer_ident)
    {
        log_error("Unable to initalize a system timer.  Nothing is going to work right.");
    }

    // Fast timer for gamepad polling (50ms for responsive button detection)
    if (!SetTimer(C_GAMEPAD_TIMER_ID, C_GAMEPAD_TIMER_MSECS, NULL))
    {
        log_error("Unable to initialize gamepad timer.");
    }
 
  
    //let's also load all the events
    app_glo.GetEventList()->LoadEvents();
 
    dll_hinstance = LoadLibrary(_T("sm.dll"));
   
    if (!install_hook(dll_hinstance, app_glo.GetHWND()))
	{
	  //hook failed.                                                 
	 log_error("Failed to create keyboard hook.");
	}
    
    SetupLog();
  
    if (!glo.m_b_boot_stealth)
    AddToTray();
    
     //first trim the file
    
     if (glo.m_b_trim_log)
     {
        if (glo.m_i_trim_lines <= 0)
        {
          LogMsg(_T("Log trimming is set to %d?  Ignoring it."), glo.m_i_trim_lines);
        } else
        {
   
          TrimTextFile(_T("toolfish_log.txt"), glo.m_i_trim_lines);
        }
      }
  
     if (exist(_T("toolfish_log.txt")))
     {
     
     //first load the log that already exists
     CTextParse parse;
     parse.open("toolfish_log.txt");
     
     //LogMsg(uni((WCHAR*)parse.GetBuffer()).GetAuto());
   
     if (parse.GetBuffByteSize() > 4)
     {
         m_cedit_log.SetSel(0, -1);
         
         //copy it into our log buffer
         m_cedit_log.ReplaceSel( (TCHAR *) (parse.GetBuffer()+2), false); //skip unicode junk
         
         //colorize everything
         
         for (int i=0; i < m_cedit_log.GetLineCount(); i++)
         {
             ColorizeLine(&m_cedit_log, i);
         }
         
     }
     }
     char st_temp[MAX_PATH];
	 app_glo.ActivateSpeech(!glo.m_b_disable_tts);
	
#ifdef _UNICODE
CHAR * st_version = "";
#else
CHAR * st_version = "(98/ME Non-Unicode Build)";

#endif 
     sprintf(st_temp, "*** %s V%.2f Started %s ***", uni(app_glo.GetAppName()).to_st(), app_glo.GetVersion(), st_version);
     log_msg(st_temp);
	
     GetLocalAddress((char*)&st_temp);
     app_glo.SetIP(st_temp);
     
    app_glo.RetrieveIPAddress(); //fill it with a valid value

     if (app_glo.GetTrialDaysLeft() < 0) 
       {
         //let's log on to rtsoft's server and check the registration code if possible
           AfxBeginThread(CheckForUpdate, 0);
  
       }
   
        BuildHotKeyInfo();
        SetupKeyboardOverlay();

        LoadButtons();
 

       return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDlgMain::LoadButtons()
{
    
     //update our graphical buttons

   m_but_events.SetTooltipText(_T("Manage all your events.  This is the most important area!"));
   m_but_events.SetBitmaps(_T("gui\\but_event.bmp"), RGB(255,255,255), 0,0);
   m_but_events.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
   m_but_events.DrawTransparent();
  // m_but_events.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
   // m_but_events.SizeToContent();
   //m_but_events.DrawBorder(false, false);
   

   
   m_but_mute.SetTooltipText(_T("Have your compute automatically lower the volume when you're away."));
   m_but_mute.DrawTransparent();
   m_but_mute.SetBitmaps(_T("gui\\but_sound.bmp"), RGB(255,255,255), 0,0);
   m_but_mute.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
 
   m_but_key_log.SetTooltipText(_T("Log all keystrokes (including passwords and email) entered and programs run."));
   m_but_key_log.DrawTransparent();
   m_but_key_log.SetBitmaps(_T("gui\\but_spy.bmp"), RGB(255,255,255), 0,0);
   m_but_key_log.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);

   
   m_but_wizard.SetTooltipText(_T("A quick and easy way to add different kinds of events.  Use the event editor to edit or setup more complicated ones."));
   m_but_wizard.DrawTransparent();
   m_but_wizard.SetBitmaps(_T("gui\\but_wizard.bmp"), RGB(255,255,255), 0,0);
   m_but_wizard.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);

   m_but_options.SetTooltipText(_T("Global Toolfish settings are here such as loading at boot or not."));
   m_but_options.DrawTransparent();
   m_but_options.SetBitmaps(_T("gui\\but_option.bmp"), RGB(255,255,255), 0,0);
   m_but_options.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);

   m_but_suggestion.SetTooltipText(_T("Desperately need a feature implemented and don't mind testing it out?  Let us know!"));
   m_but_suggestion.SetURL(_T("http://rtsoft.com/pages/feedback.php?game=Toolfish&version=2.30"));
   m_but_suggestion.DrawTransparent();
   m_but_suggestion.SetBitmaps(_T("gui\\but_suggest.bmp"), RGB(255,255,255), 0,0);
   m_but_suggestion.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
  

   m_but_hide.SetTooltipText(_T("Minimize toolfish to the tray."));
   m_but_hide.DrawTransparent();
   m_but_hide.SetBitmaps(_T("gui\\icon_tray.bmp"), RGB(255,255,255), 0,0);
   m_but_hide.SetAlign(CButtonST::ST_ALIGN_HORIZ, FALSE);

   
   m_but_email.SetTooltipText(_T("This is an easy way to see all emails still sitting on the server(s)."));
   m_but_email.DrawTransparent();
   m_but_email.SetBitmaps(_T("gui\\icon_email.bmp"), RGB(255,255,255), 0,0);
   m_but_email.SetAlign(CButtonST::ST_ALIGN_HORIZ, FALSE);

   m_but_fun.SetTooltipText(_T("Just for fun stuff like usage statistics and Leet-Type."));
   m_but_fun.DrawTransparent();
   m_but_fun.SetBitmaps(_T("gui\\icon_fun.bmp"), RGB(255,255,255), 0,0);
   m_but_fun.SetAlign(CButtonST::ST_ALIGN_HORIZ, FALSE);

                     

   m_but_help.SetTooltipText(_T("Get the most of Toolfish by reading the help.  Some stuff is tricky."));
   m_but_help.DrawTransparent();
   m_but_help.SetBitmaps(_T("gui\\icon_help.bmp"), RGB(255,255,255), 0,0);
   m_but_help.SetAlign(CButtonST::ST_ALIGN_HORIZ, FALSE);
 
    m_but_clear.DrawBorder(false, false);
  // m_but_clear.SetURL(L"http://www.clear.com")	;
   m_but_clear.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(20,20,255), false);
   m_but_clear.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0), false);
    m_but_clear.DrawTransparent();


    m_but_logo.SetTooltipText(_T("Click here for the about screen."));
    m_but_logo.DrawTransparent();
    m_but_logo.SetBitmaps(_T("gui\\logo_small.bmp"), RGB(255,255,255), 0,0);
    m_but_help.SetAlign(CButtonST::ST_ALIGN_HORIZ, FALSE);
 
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlgMain::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBkDialogST::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgMain::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CDlgMain::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

    if (message == WM_NOTIFY)
    {
       LPNMHDR lpNmhdr = (LPNMHDR)lParam;
            switch ( lpNmhdr->code)
            {
            case  EN_LINK:
                {
                    ENLINK *pEnLink = (ENLINK *)lParam;
                    
                    if(pEnLink->msg == WM_LBUTTONUP)
                    {
                        WCHAR str[C_MAX_URL_SIZE];


                        TEXTRANGEW tr;
                        tr.chrg.cpMin = pEnLink->chrg.cpMin;
                         tr.chrg.cpMax = pEnLink->chrg.cpMax;
                         tr.lpstrText = str;
                         m_cedit_log.SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
                  
                         #ifdef _UNICODE
                         LaunchURL(str);
#else
                         LaunchURL(uni(str).to_st());
#endif

                         
                         /*
                         winall_create_url_file_full(uni(str).to_st());

                         int result = (int)ShellExecute(NULL,_T("open"),_T("temp.url"), NULL,NULL, SW_MAXIMIZE);
#ifdef _UNICODE
                         ProcessError(this->m_hWnd, result, _T("temp.url"),str); //show an error message if we couldn't open this
#else
                         ProcessError(this->m_hWnd, result, _T("temp.url"),uni(str).to_st()); //show an error message if we couldn't open this

#endif
                           */
                    }
                }	
            }
    }
    // TODO: Add your specialized code here and/or call the base class
	return CBkDialogST::WindowProc(message, wParam, lParam);
}

bool ColorizeWord(CRichEditCtrlEx * p_rich_edit, CHARRANGE *p_range, const WCHAR *p_line, WCHAR *p_word, COLORREF color, bool b_bold)
{
    
   // OutputDebugString(p_line);
    WCHAR *p_first_char;
    if ( p_first_char = (WCHAR*)wcsstr(p_line, p_word))
    {
    
         int i_index = p_first_char-p_line;
   
        //found the word.
        CHARFORMATW cfColor; //force unicode version, even on ANSI build
        ZeroMemory(&cfColor, sizeof(CHARFORMATW));
        cfColor.cbSize =sizeof(CHARFORMATW);
        
        //the color we want
        cfColor.dwMask = CFM_COLOR;
        cfColor.crTextColor = color;
            
            if (b_bold)
            {
                cfColor.dwMask = CFM_COLOR|CFM_BOLD;
                cfColor.dwEffects = CFE_BOLD;
            }
        //    char st_crap[1024];
        //    sprintf(st_crap, "min is %d, max is %d.", p_range->cpMin+i_index, p_range->cpMin+i_index+wcslen(p_word));
        //    log_error(st_crap);

             //move the selection a bit
           p_rich_edit->SetSel(p_range->cpMin+i_index, p_range->cpMin+i_index+wcslen(p_word));
          
//            if (!p_rich_edit->SetSelectionCharFormat((CHARFORMATA)cfColor))

            //color it
            if (p_rich_edit->SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cfColor ) == 0)
            {
                LogError(_T("Error setting color."));
            }
        //remove selection so we don't see that annoying box
            //p_rich_edit->SetSel();
            
    }
    

    return true;
}

void ColorizeLine(CRichEditCtrlEx * p_rich_edit, int i_line_to_color)
{
     
 #ifdef _UNICODE 
    GETTEXTLENGTHEX gtlx = { GTL_NUMCHARS, CP_THREAD_ACP };
#else
    GETTEXTLENGTHEX gtlx = { GTL_NUMCHARS, CP_THREAD_ACP };
    
#endif
    
    

   int i_total_length = p_rich_edit->SendMessage(EM_GETTEXTLENGTHEX, (WPARAM) &gtlx, 0);
   
    int i_start_position;
    int i_end_position;
    
    if (i_line_to_color+1 == p_rich_edit->GetLineCount())
    {
        //special code for handling the last line...
           i_start_position = p_rich_edit->LineIndex(i_line_to_color-1);
           i_end_position = i_total_length;

    }
    else
    {
    
 
    i_start_position = p_rich_edit->LineIndex(i_line_to_color);
    i_end_position = i_start_position + p_rich_edit->LineLength(i_line_to_color);

    }

    CHARRANGE range;
    range.cpMin = i_start_position;
    range.cpMax = i_end_position;
    
    p_rich_edit->SetSel(range);

    WCHAR st_temp[1024];

#ifdef _UNICODE
    p_rich_edit->GetSelText((char*)st_temp);
#else
    p_rich_edit->GetSelText((char*)st_temp);
    
#endif
  //  OutputDebugString(st_temp);

    //do our coloring thing
    ColorizeWord(p_rich_edit, &range, st_temp, L"PAGE TRIGGERED:", RGB(255,0,0), true);
    ColorizeWord(p_rich_edit, &range, st_temp, L"RUNNING:", RGB(0,170,0), true);
    ColorizeWord(p_rich_edit, &range, st_temp, L"NEW EMAIL ARRIVED:", RGB(160,0,255), true);
    ColorizeWord(p_rich_edit, &range, st_temp, L"SITE MONITOR FAILED:", RGB(255,0,0), true);
   
    
  //  add_text(uni(st_temp).to_st(), "debug.txt");
   // p_rich_edit->SetSel(i_end_position,i_end_position); //clear the selection
    
}



void CDlgMain::Log(CString cst_message)
{
   	
    CString cst_date;
    cst_date.Format(_T("%s - %s: "), show_small_date(), show_time());
    cst_date += cst_message;
    AddText(cst_date, "toolfish_log.txt");
    
    int i_lines = 0;
    
    //add it
#ifdef _UNICODE 
    GETTEXTLENGTHEX gtlx = { GTL_NUMCHARS, CP_THREAD_ACP };
#else
    GETTEXTLENGTHEX gtlx = { GTL_NUMCHARS,CP_THREAD_ACP };
    
#endif
     
     int i_total_length = m_cedit_log.SendMessage(EM_GETTEXTLENGTHEX, (WPARAM) &gtlx, 0);
   

    
    m_cedit_log.SetSel(i_total_length, i_total_length);
    //copy it into our log buffer
#ifdef _UNICODE
    m_cedit_log.ReplaceSel( cst_date, false); //skip unicode junk
#else
    m_cedit_log.ReplaceSel( (TCHAR*)uni(cst_date).us_data, false); //skip unicode junk
    
#endif
     i_lines = m_cedit_log.GetLineCount();

   if (i_lines > C_MAX_LOG_LINES)
   {
       int i_lines_to_trim = i_lines-C_MAX_LOG_LINES;

       m_cedit_log.SetSel(0, m_cedit_log.LineIndex(i_lines));
       m_cedit_log.ReplaceSel(_T(""));

   }

   
 ColorizeLine(&m_cedit_log, m_cedit_log.GetLineCount()-1);
    /*
    int i_len = m_cedit_log.SendMessage(EM_GETTEXTLENGTHEX, (WPARAM) &gtlx, 0);
  
    m_cedit_log.SetSel(0,0);
    m_cedit_log.SendMessage(EM_LINESCROLL,0, i_len-20 );
    /*
    m_cedit_log.SetSel(i_len-1,i_len);
    m_cedit_log.SendMessage(EM_SCROLLCARET);

      */

  //unselect the text
   int i_len = m_cedit_log.SendMessage(EM_GETTEXTLENGTHEX, (WPARAM) &gtlx, 0);
   m_cedit_log.SetSel(i_len,i_len);

}

void CDlgMain::OnEvents() 
{
    //they want to edit the events
     CDlgEvents dlg;
     dlg.DoModal();

     //I save this here because the default font might have been changed
     FileConfigSave(&glo);

}

void CDlgMain::OnClose() 
{

    if (glo.m_b_minimize_on_close)
    {
      PostMessage(WM_HIDE, 0,0);
      return;
    }
    
    CDlgQuit dlg;
    int i_res = dlg.DoModal();

if (i_res != IDOK)  return;


    FinalShutDown();
	//CBkDialogST::OnClose();
}

void CDlgMain::FinalShutDown()
{

      if (app_glo.GetActiveThreadCount() > 0)
    {
        CDlgThread dlg_thread;
        dlg_thread.DoModal();
    }

    //stop the dll from talking to us anymore
      DeactivateMute();
	  ShutDownStuff(); 
      kill_hooks();
      RemoveFromTray();
      KillBitmaps();
	//save settings
      FileConfigSave(&glo);
      FreeLibrary(dll_hinstance);
      DestroyIcon(m_hIcon_small);
      //LogMsg("Destroyed stuff");
      DestroyWindow();
    //  delete this;

}

void CDlgMain::ShutDownStuff()
{
    if (app_glo.m_timer_ident)
    {
        KillTimer(app_glo.m_timer_ident);
        app_glo.m_timer_ident = 0;                     
    }

    // Kill gamepad polling timer
    KillTimer(C_GAMEPAD_TIMER_ID);
}
void CDlgMain::KillBitmaps()
{
}

void CDlgMain::OnDestroy() 
{
	//CBkDialogST::OnDestroy();
}


LRESULT CDlgMain::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
      if (message == WM_APP_OPENED)
        {
          ProcessOpenedWindow((HWND)wParam); //send the handle
        }

      if (message == WM_ADD_TO_LOG_DLL)
	{
	  char *temp = (char *) lParam;
	  if (temp)
      {
      
          this->Log(temp);
	 //don't free the memory, it was made in the dll, the dll can handle it
    } else                      
      {
          Log("Got a null!");
      }
	}

     

    if (message == WM_TIMER)
    {
        if (wParam == 1)
        {
         //that's our baby!
        //run through all events
         ProcessTimerEvents();
         app_glo.ShutDownMessageCheck();
         this->ProcessTray(); //do the tray blinking
       
         if (m_timer_auto_save.interval_reached())
         {
             AutoSave();
             //see if our IP address changed
         }

        if (m_timer_ip_check.interval_reached())
        {
            app_glo.RetrieveIPAddress();
        }

         if (m_timer_maint.interval_reached())
         {
             ProcessMaintCheck();
         }


		 //test area

#ifdef _DEBUG

//LogMsg("TESTING");
#endif
  
		 //AdjustMic("2- H4", 0.08f);
        }

        // Fast gamepad polling timer (50ms)
        if (wParam == C_GAMEPAD_TIMER_ID)
        {
            if (CheckGamepadActivity())
            {
                ProcessMuteGamepad();
            }
        }
    }

    if (message == WM_SAY)
    {
        TCHAR *temp = (TCHAR *) lParam;
        if (app_glo.GetSpeech())
        {
			//CVoice test;
			//test.SayTest(temp);
			glo.m_voice.SayTest(temp);
        }
        
        SAFE_DELETE_ARRAY(temp);
    }
   if (message == WM_GOTKEY)
    {
       glo.m_stats_today.AddKey();
       glo.m_stats_total.AddKey();

       ProcessMuteKey();
       ProcessVolumeKnobKey(wParam);
       ProcessGotKey(wParam, lParam);
   }                 
    if (message == WM_GOTMOUSE)        
    {
        glo.m_stats_today.AddMouse(lParam + wParam);
        glo.m_stats_total.AddMouse(lParam + wParam);
        
        ProcessMuteMouse();
    }

    if (message == WM_MOUSECLICK)
    {
        //global mouse click thingie
        if (wParam == WM_RBUTTONDOWN)
        {
            glo.m_stats_today.AddMouseButton(C_MOUSE_RIGHT);
            glo.m_stats_total.AddMouseButton(C_MOUSE_RIGHT);
        }
        if (wParam == WM_LBUTTONDOWN)
        {
            glo.m_stats_today.AddMouseButton(C_MOUSE_LEFT);
            glo.m_stats_total.AddMouseButton(C_MOUSE_LEFT);
        }
        if (wParam == WM_MBUTTONDOWN)
        {
            glo.m_stats_today.AddMouseButton(C_MOUSE_MIDDLE);
            glo.m_stats_total.AddMouseButton(C_MOUSE_MIDDLE);
        }
        if (wParam == WM_MOUSEWHEEL)
        {
            glo.m_stats_today.AddMouseButton(C_MOUSE_WHEEL);
            glo.m_stats_total.AddMouseButton(C_MOUSE_WHEEL);
        }

    }
    if (message == WM_TOGGLE_TTS)
    {
        //toggle voice on/off
        glo.m_b_disable_tts = !glo.m_b_disable_tts;
        
        if (glo.m_b_disable_tts)
        {
            app_glo.ActivateSpeech(false);
            
        }   else
        {
            app_glo.ActivateSpeech(true);
            
        }
        
        
    }
    

    if (message == WM_TOGGLE_STEALTH)
    {
        if (g_b_tray_active)
        {
            LogMsg(_T("Tray icon hidden.  Hit again to unhide it."));
            RemoveFromTray();
        } else
        {
            AddToTray();
            ShowWindow(SW_SHOWDEFAULT);
            SendMessage(WM_ACTIVATE);  
            SetForegroundWindowEx(m_hWnd); //  this->SetActiveWindow();
            g_b_minimized = false;
            LogMsg(_T("Tray icon unhidden and Toolfish opened."));
            BringWindowToTop();
            
        }
        
    }
  
    if (message == WM_TRAY_BLINK_ON)
    {
        TrayStartBlink(lParam);
    }
    
    
     if (message == WM_TRAY_BLINK_OFF)
    {
        TrayStopBlink(lParam);
    }
  
    if (message == WM_HIDE)
   {
        g_b_minimized = true;
    ShowWindow(SW_HIDE);
   }         

   if (message == WM_MY_TRAY_NOTIFICATION)
   {
     //   LoadButtons();
   
       ShowWindow(SW_SHOW);
   }

   if (message == WM_TOGGLE_TOOLFISH)
   {
       //let's toggle this sucker
       if (g_b_minimized)
       {
           if (g_b_tray_active)
           {
             ShowWindow(SW_SHOWDEFAULT);
            SendMessage(WM_ACTIVATE);  
            SetForegroundWindowEx(m_hWnd); //  this->SetActiveWindow();
              BringWindowToTop();
            g_b_minimized = false;
           }
       }  else
       {
           g_b_minimized = true;

           ShowWindow(SW_HIDE);

       }


   }

   if (message == WM_ADD_TO_LOG)
   {
       char *temp = (char *) lParam;
       if (temp)
       {
           this->Log(temp);
           //free the memory that was created to pass this string
           SAFE_FREE(temp);
       } else
       {
           Log("Got a null!");
       }
   }
   
   
   if (message == WM_ADD_TO_LOGW)
   {
       //same as above but for unicode chars.  But sending a TCHAR to Log() it will
       //know to it is unicode, that's the only difference.
       WCHAR *temp = (WCHAR *) lParam;
       if (temp)
       {
           this->Log(temp);
           //free the memory that was created to pass this string
           SAFE_FREE(temp);
       } else
       {
           Log("Got a null!");
       }
   }

    
    return CBkDialogST::DefWindowProc(message, wParam, lParam);
}

void CDlgMain::SetupLog()
{
     //let's send over the new stuff to the log
     g_key_log.SetFileName(uni(glo.m_st_key_log).to_st());
     g_key_log.LogActive(glo.m_b_key_log_enabled);

}

void CDlgMain::OnKeyLog() 
{
	CDlgKeyLog dlg;
    dlg.DoModal();
    SetupLog();
}

void CDlgMain::OnOptions() 
{
   CDlgOptions dlg;	
   dlg.DoModal();
}

void CDlgMain::TrayStartBlink(long i_code)
{
  
    if (glo.m_b_tray_disable_blinking) return; //no blinking
    
    if (g_b_minimized) 
   {
   
    if (m_i_current_tray_mode == C_TRAY_MODE_NORMAL)
    {
      //need to activate the blinking.
      m_timer_tray.TriggerInterval(); //make it start blinking NOW
      m_i_current_tray_mode = C_TRAY_MODE_BLINK;
    }
   }

}

void CDlgMain::TrayStopBlink(long i_code)
{
    m_i_current_tray_mode = C_TRAY_MODE_NORMAL;
}


void CDlgMain::ProcessTray()
{
    //to make the tray blink or update it's text, let's do this
    //assuming it should blink
   
    
    if (!g_b_tray_active || !m_timer_tray.interval_reached())
    {
        return; //not time yet
    }

    if (!g_b_minimized)
    {
        TrayStopBlink(0);
    }

    //figure out what we're doing
   
    if (
        (m_i_current_tray_mode == C_TRAY_MODE_NORMAL)
        && (m_i_current_tray_icon == C_TRAY_ICON_NORMAL)
        )
    {
        return;
    }

    
    //toggle it
  
	NOTIFYICONDATA notify_struct;
	//set the info for the struct
	notify_struct.cbSize = sizeof(NOTIFYICONDATA);
	notify_struct.hWnd = this->m_hWnd;
	notify_struct.uID = WM_MY_TRAY_NOTIFICATION;
	notify_struct.uFlags = NIF_ICON;

    if (m_i_current_tray_icon == C_TRAY_ICON_ALARM)
    {
        
        notify_struct.hIcon = m_hIcon_small;
        m_i_current_tray_icon = C_TRAY_ICON_NORMAL;
    } else
    {
        notify_struct.hIcon = m_hIcon_small_alarm;
        m_i_current_tray_icon = C_TRAY_ICON_ALARM;
        
    }
    Shell_NotifyIcon(NIM_MODIFY, &notify_struct);	
 	this->UpdateWindow();
    g_b_tray_active = true;
}


void CDlgMain::AddToTray()
{
   if (g_b_tray_active) return; //already active
	NOTIFYICONDATA notify_struct;

	//set the info for the struct
	notify_struct.cbSize = sizeof(NOTIFYICONDATA);
	notify_struct.hWnd = this->m_hWnd;
	notify_struct.uID = WM_MY_TRAY_NOTIFICATION;
	notify_struct.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
	notify_struct.uCallbackMessage = WM_MY_TRAY_NOTIFICATION;
	notify_struct.hIcon = m_hIcon_small;
	m_i_current_tray_icon = C_TRAY_ICON_NORMAL;
    wsprintf(notify_struct.szTip, app_glo.GetAppName());
	Shell_NotifyIcon(NIM_ADD, &notify_struct);	
 	this->UpdateWindow();
    g_b_tray_active = true;
}

void CDlgMain::RemoveFromTray()
{
    if (!g_b_tray_active) return; //don't need to remove
	NOTIFYICONDATA notify_struct;

	//set the info for the struct
    notify_struct.cbSize = sizeof(NOTIFYICONDATA);
	notify_struct.hWnd = this->m_hWnd;
	notify_struct.uID = WM_MY_TRAY_NOTIFICATION;
	notify_struct.hIcon = m_hIcon;
	Shell_NotifyIcon(NIM_DELETE, &notify_struct);	
    //ShowWindow(SW_SHOW);
	this->UpdateWindow();
    g_b_tray_active = false;
}



void CDlgMain::OnSysCommand(UINT nID, LPARAM lParam) 
{
    // TODO: Add your message handler code here and/or call default
    unsigned int uCmdType =nID & 0xFFF0;
    
    if (uCmdType == SC_MINIMIZE)
    {
        if (g_b_tray_active)
            AddToTray();
        PostMessage(WM_HIDE);
    }
    
    CBkDialogST::OnSysCommand(nID, lParam);
}

void CDlgMain::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CBkDialogST::CalcWindowRect(lpClientRect, nAdjustType);
}


BOOL SetForegroundWindowEx(HWND hWnd)
{
     BOOL res = TRUE;

    // Attempt to call SetForegroundWindow()
     if (!SetForegroundWindow(hWnd))
     {
          BOOL res = FALSE;

          // Here we try to find foreground window
          HWND hForegroundWnd = GetForegroundWindow();

          // if there is no any foreground window, then input focus on theTaskBar.
          if (!hForegroundWnd) hForegroundWnd = FindWindow(_T("Shell_TrayWnd"), NULL);

          if (hForegroundWnd)
          {
                DWORD ThreadIdAttachTo = GetWindowThreadProcessId(hForegroundWnd, NULL);

                // Attaching to the input thread
               AttachThreadInput(GetCurrentThreadId(), ThreadIdAttachTo, TRUE);

               // Doing our job
               if (SetForegroundWindow(hWnd)) res = TRUE;

                // Detaching from the input thread.
               AttachThreadInput(GetCurrentThreadId(), ThreadIdAttachTo, FALSE);
          }
     }
     return res;
}


LRESULT CDlgMain::OnTrayNotification(WPARAM uID, LPARAM lEvent)

{  
    if (lEvent == WM_RBUTTONDOWN)
    {
        POINT pt;
        GetCursorPos(&pt);
        
        CMenu menu;
        menu.LoadMenu(IDR_TRAY_MENU);
        CMenu* popup = menu.GetSubMenu(0);
        if (popup)
        {
            //force to front
            ::SetForegroundWindow(app_glo.GetDialogHWND());
            if (glo.m_b_disable_tts)
            popup->CheckMenuItem(ID_MENU_VOICE_MUTE, MF_CHECKED);
            
            if (!glo.m_b_smart_mute)
            {
            popup->CheckMenuItem(ID_DISABLE_SMARTMUTE, MF_CHECKED);
            }

        /*
            if (app_glo.m_b_keyboard_disabled)
            {
                popup->CheckMenuItem(ID_TRAY_DISABLE_KEYBOARD, MF_CHECKED);
            }
        */

       popup->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, this);
        } else
        {
            log_error("Popup was null.");
        }
    }
    
    
    if (lEvent == WM_LBUTTONDOWN)
    {
        SetTrayBlinking(false, 0); //the last number isn't used yet
      
        OpenDialogFromTray();
    }
    
    return 0;
}

void CDlgMain::OpenDialogFromTray()
{
  
    ShowWindow(SW_SHOWNORMAL);
    SendMessage(WM_ACTIVATE);  
    BringWindowToTop();
    SetForegroundWindowEx(m_hWnd); //  this->SetActiveWindow();
    g_b_minimized = false;
}

void CDlgMain::OnOk() 
{
	ShowWindow(SW_MINIMIZE);
    PostMessage(WM_HIDE);
}

void CDlgMain::OnAbout() 
{
    CDlgAbout dlg;
    dlg.DoModal();
}

void CDlgMain::OnMute() 
{
    CDlgMute dlg;
    dlg.DoModal();
}

void CDlgMain::OnMenuExit() 
{
  if (!app_glo.GetPaused())	
  {
      FinalShutDown(); 
  }
}

void CDlgMain::OnMenuOptions() 
{
	OpenDialogFromTray();
}

HBRUSH CDlgMain::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
	
    switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
        if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
          //    pDC->SetBkMode(TRANSPARENT);
            pDC->SetBkColor(RGB(255,255,255));
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }

        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
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


void CDlgMain::OnPostEraseBkgnd(CDC* pDC)
{
    this->m_but_email.SetBk(pDC);

    m_but_events.SetBk(pDC);

    m_but_mute.SetBk(pDC);

    m_but_key_log.SetBk(pDC);
    m_but_wizard.SetBk(pDC);
    m_but_options.SetBk(pDC);
    m_but_suggestion.SetBk(pDC);
    m_but_hide.SetBk(pDC);
    m_but_fun.SetBk(pDC);
    m_but_help.SetBk(pDC);
    m_but_clear.SetBk(pDC);
    m_but_logo.SetBk(pDC);
    
}


void CDlgMain::OnWizard() 
{
	// TODO: Add your control notification handler code here
  
       POINT pt;
        GetCursorPos(&pt);
        
        CMenu menu;
        menu.LoadMenu(IDR_MENU_WIZARD);
        CMenu* popup = menu.GetSubMenu(0);
        if (popup)
        {
            //force to front
            ::SetForegroundWindow(app_glo.GetDialogHWND());
            
            popup->TrackPopupMenu(TPM_RIGHTBUTTON|TPM_LEFTALIGN, pt.x, pt.y, this);
        } else
        {
            log_error("Popup was null.");
        }
   
}

void CDlgMain::OnClear() 
{
         m_cedit_log.SetSel(0, -1);
          //copy it into our log buffer
         m_cedit_log.ReplaceSel( _T(""), false); //skip unicode junk
         //also kill the log
         _tunlink(_T("toolfish_log.txt"));

}

void CDlgMain::OnLogo() 
{
    CDlgAbout dlg;
    dlg.DoModal();
	
}

void CDlgMain::PostNcDestroy() 
{
	delete this;
}

void CDlgMain::OnButHelp() 
{

 int result = (int)ShellExecute(NULL,_T("open"),_T("docs\\help.htm"), NULL,NULL, SW_MAXIMIZE);
  ProcessError(this->m_hWnd, result, _T("temp.url"),_T("docs\\help.htm")); //show an error message if we couldn't open this
}

void CDlgMain::OnMenuWizardWeb() 
{
	// TODO: Add your command handler code here
 CDlgWizard dlg;
    dlg.DoModal();
	
}

void CDlgMain::OnMenuWizardOneTimeReminder() 
{
    // TODO: Add your command handler code here
    CDlgWizardOTReminder dlg;
    dlg.DoModal();
}

void CDlgMain::OnMenuVoiceMute() 
{
	//send the main app a message to turn off or on the sound, can't do it in this thread, can causes memory
    //problems because of the  CoUninitialize() that gets done
    
           PostMessage(WM_TOGGLE_TTS);

 

}

void CDlgMain::OnAboutToolfish() 
{
	OnAbout();
}

void CDlgMain::OnDisableSmartmute() 
{
	glo.m_b_smart_mute = !glo.m_b_smart_mute;
}

void CDlgMain::OnMenuWizardReminder() 
{
    CDlgWizardRecurrentReminder dlg;
    dlg.DoModal();
}

void CDlgMain::OnMenuWizardAtomic() 
{
	// TODO: Add your command handler code here
	CDlgWizardAtomic dlg;
    dlg.DoModal();
}

void CDlgMain::OnMenuReadClipboard() 
{
   //let's read it!
    
    //ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);
  
    CString strText;
    
    OpenClipboard();
    if (!IsClipboardFormatAvailable(CF_TEXT))
    {
        CloseClipboard();
        app_glo.ActivateSpeech(true);
        TCHAR *p_message = _T("Can't read the clipboard, there is no text data in it right now.\r\n\r\nHighlight some text, then Ctrl-C (or right-click and choose Copy) and then try this again.");
        CDlgAlert dlg;
        dlg.StoreTextPointerForLater(p_message); //WCHAR format
        dlg.SetActionPointer(NULL);
        dlg.DoModal();
        
        
        return;
    }
    else
    {
#ifdef _UNICODE
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);

#else
        HANDLE hData = GetClipboardData(CF_TEXT);

#endif
        
        LPSTR lpData = (LPSTR)GlobalLock(hData);
        
      //  strText.Format("%s",lpData);
        

        if (app_glo.GetSpeech())
        {
          //SAPI won't work in a thread, needs the message pump for messaging
          //Let's send it to our main window to play
          //global say will make a copy and send it
           glo.m_voice.SayTest((TCHAR*)hData);
        } else
        {
        LogError(_T("Microsoft SAPI has been disabled from the options menu, can't use text to speech."));
        }

        GlobalUnlock(hData);
    }
    CloseClipboard();
    

}

void CDlgMain::OnMenuWizardEmail() 
{
    CDlgWizardEmail dlg;
    dlg.DoModal();
}

void CDlgMain::OnEmailview() 
{
	CDlgMailView dlg;
    dlg.DoModal();
}

void CDlgMain::OnMenuWizardMonitor() 
{
    CDlgWizardMonitor dlg;
    dlg.DoModal();
	
}

void CDlgMain::OnFun() 
{
	CDlgFun dlg;
    dlg.DoModal();
}
/*

void CDlgMain::OnTrayDisableKeyboard() 
{
	// TODO: Add your command handler code here

    if (app_glo.m_b_keyboard_disabled)
    {
            LogMsg(_L("Keyboard re-activated."));
           app_glo.m_b_keyboard_disabled = false;
    }       else
    {
       DlgOkCancel dlg;
        dlg.SetTitle(_T("Keyboard Clean"));
        dlg.SetDialogText(_T("Click ok to disable the keyboard.  When finished, right click the Toolfish tray icon\r\nand toggle this option back off."));
        if (dlg.DoModal() == IDOK)
        {
            LogMsg(_L("Keyboard disabled for cleaning."));
            app_glo.m_b_keyboard_disabled = true;
        } else
        {
        }                            
                              
    }

           SetDisableKeyboard(app_glo.m_b_keyboard_disabled);

}  
  */

/*

SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE,
&dwNotUsedForAnything, 0);

   // disable
   SystemParametersInfo(SPI_SCREENSAVERRUNNING, FALSE,
&dwNotUsedForAnything, 0);


  */

 
 

void CDlgMain::OnMenuWizardReminder1Day() 
{
   CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_DAY);
}

void CDlgMain::OnMenuWizardReminder1Hour() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_HOUR);
	
}

void CDlgMain::OnMenuWizardReminder1Month() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MONTH);  //kind of assuming a month is 30 days here

}

void CDlgMain::OnMenuWizardReminder1Week() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_WEEK);  
}

void CDlgMain::OnMenuWizardReminder10() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MINUTE * 10);  
	
}

void CDlgMain::OnMenuWizardReminder15() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MINUTE * 15);  
	
}

void CDlgMain::OnMenuWizardReminder2Hour() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_HOUR * 2);  
	
}

void CDlgMain::OnMenuWizardReminder30() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MINUTE * 30);  
	
}

void CDlgMain::OnMenuWizardReminder4Hours() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_HOUR * 4);  
	
}

void CDlgMain::OnMenuWizardReminder5() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MINUTE * 5);  
	
}

void CDlgMain::OnMenuWizardReminder8Hours() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_HOUR * 8);  
    
	
}

void CDlgMain::OnMenuWizardReminder45() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MINUTE * 45);  
    

}

void CDlgMain::OnMenuWizardReminder20() 
{
    CDlgWizardReminderX dlg;
    dlg.Init(C_ONE_MINUTE * 20);  
    

}
