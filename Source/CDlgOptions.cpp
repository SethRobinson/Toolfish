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
    m_b_enable_tray = !glo.m_b_tray_disable_blinking;
    m_b_enable_blink_webpage = !glo.m_b_tray_no_trigger_on_webcompare;
    m_b_enable_blink_email = !glo.m_b_tray_no_trigger_on_email;
    m_b_detect_IP_address_server_side = !glo.m_b_server_ip_disabled;
    m_b_minimize_on_close = glo.m_b_minimize_on_close;

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
   
   glo.m_b_tray_disable_blinking = m_b_enable_tray != 1; //reversed on purpose
   glo.m_b_tray_no_trigger_on_webcompare = m_b_enable_blink_webpage != 1; //reversed on purpose
   glo.m_b_tray_no_trigger_on_email =  m_b_enable_blink_email != 1; //reversed
   glo.m_b_server_ip_disabled =  m_b_detect_IP_address_server_side != 1; //reversed
   glo.m_b_minimize_on_close = m_b_minimize_on_close != 0;
   
   ProcessGray();

   global_registry(false, glo.m_b_boot_load);
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

void CDlgOptions::OnSmtpConfigure() 
{
    CSMPTSetup dlg;
    dlg.DoModal();

}

void CDlgOptions::OnBEnableTray() 
{
    ProcessGray();
}

