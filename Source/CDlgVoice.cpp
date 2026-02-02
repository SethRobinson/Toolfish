// CDlgVoice.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgVoice.h"
#include "..\Shared\uni.h"
#include "..\Shared\mfc_all.h"
#include "ProcessEvents.h"
#include "CVoice.h"
#include "..\Shared\registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgVoice dialog


CDlgVoice::CDlgVoice(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgVoice::IDD, pParent)
{
    m_p_voice = NULL;
    //{{AFX_DATA_INIT(CDlgVoice)
	m_cst_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgVoice::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVoice)
	DDX_Control(pDX, IDC_PITCH_SPEED, m_slider_speed);
	DDX_Control(pDX, IDC_PITCH_SLIDER, m_slider_pitch);
	DDX_Text(pDX, IDC_TEXT, m_cst_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVoice, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgVoice)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_HEAR, OnHear)
	ON_BN_CLICKED(ID_SPEECH_PROPERTIES, OnSpeechProperties)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVoice message handlers

HBRUSH CDlgVoice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
    
    switch (nCtlColor)
    {
        
    case CTLCOLOR_STATIC:  
        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
            ||
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
            ||
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
            )
        {
            pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) m_hbrush;
            return hbr;
        }
        
        pDC->SetBkMode(TRANSPARENT);
        // hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
        hbr=(HBRUSH) m_hbrush;
        return hbr;
        break;
    case CTLCOLOR_BTN:
        pDC->SetBkMode(TRANSPARENT);
        hbr=(HBRUSH) m_hbrush;
        return hbr;
        
        break;
        
    }
    return hbr;
}

BOOL CDlgVoice::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	if (SetBitmap(_T("gui\\background.bmp")) != BKDLGST_OK)
    {
        log_error("Unable to load dialog background.");
    }
	int i_random = random_range(1, 5);

    if (i_random == 1)  m_cst_text = L"Testing 1 2 3";
    if (i_random == 2)  m_cst_text = L"I've got mail! I've got mail!  Yaaaaah.";
    if (i_random == 3)  m_cst_text = L"Hello, I have a funny computer voice.";
    if (i_random == 4)  m_cst_text = L"Toolfish let's you work smarter.";
    if (i_random == 5)  m_cst_text = L"Do not fear mistakes, there are none.  (Miles Davis)";
    
    
    m_slider_pitch.SetRange(-30, 30, TRUE);
	m_slider_pitch.SetPos(m_p_voice->GetPitch());
    m_slider_pitch.SetTicFreq(3);
    m_slider_speed.SetRange(-10, 10, TRUE);
	m_slider_speed.SetPos(m_p_voice->GetSpeed());
    m_slider_speed.SetTicFreq(1);

    
    UpdateData(D_TO_WINDOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgVoice::OnOK() 
{
    //copy it to the real one
    m_p_voice->SetPitch(m_slider_pitch.GetPos());
    m_p_voice->SetSpeed(m_slider_speed.GetPos());


    
	CBkDialogST::OnOK();
}

void CDlgVoice::OnHear() 
{
    UpdateData(D_TO_VAR);
    CVoice test;
    test.SetPitch(m_slider_pitch.GetPos());
    test.SetSpeed(m_slider_speed.GetPos());

    //say with the chosen voice
    
    test.SayTest((const TCHAR*)m_cst_text);
}

void CDlgVoice::OnSpeechProperties() 
{
 
    // TODO: Add your control notification handler code here
    SHELLEXECUTEINFOA ExecInfo;
    ZeroMemory(&ExecInfo, sizeof(SHELLEXECUTEINFO));
    ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
     CHAR wst_file[MAX_PATH];

#ifdef _USDSDNICODE

    //first get the directory of SAPI from the registry
   	 CRegistry reg;
	 CString sRegPath("Control Panel\\MMCPL");	

    if (!reg.Open(HKEY_CURRENT_USER, sRegPath))
    {
        LogError(_T("Can't open Speech properties.  It appears you have not installed the Speech API yet."));
          return;
    }

    //get the path
    CString cst_file;
    if (!reg.ReadString(_T("sapi.cpl"), cst_file))
    {
          LogError(_T("Can't open Speech properties.  It appears you have not installed the Speech API yet."));
        return;

    }
   _tcscpy (wst_file, (const TCHAR*)cst_file);

#else
 //the win 98 way
   char st_windows[MAX_PATH];
   GetWindowsDirectoryA(st_windows,MAX_PATH);
   strcpy(wst_file, "\"C:\\Program Files\\Common Files\\Microsoft Shared\\Speech\\SAPI.CPL\"");
   //replace HD letter if needed
   wst_file[1] = st_windows[0];

 //  log_msg(wst_file);
#endif
    
    ExecInfo.lpFile = wst_file;
    ExecInfo.nShow = SW_SHOWDEFAULT;

#ifdef _BLAHBLAHUNICODE
ExecInfo.fMask = SEE_MASK_UNICODE;
#endif

#ifdef _DSDUNICODE
if (!ShellExecuteEx(&ExecInfo))
{

   int i_result = (int)ExecInfo.hInstApp; 
    
     //there was a problem.
    if (i_result == ERROR_FILE_NOT_FOUND)
    {
        LogError(_T("Can't open speech properties, file not found. Have you installed the Speech API yet?"));
    }   else
    
    if (i_result == ERROR_FILE_NOT_FOUND)
    {
        LogError(_T("Can't open speech properties, path not found.  Try using your control panel instead."));
    }   else
    
    {
        LogError(_T("Can't open speech properties, unknown error.  Try using your control panel instead."));
    } 
 
}
    
#else

CHAR st_run_command[MAX_PATH];
sprintf(st_run_command, "shell32.dll,Control_RunDLL %s", wst_file);

int i_result = (int)	ShellExecuteA(NULL,		
		"open",	
		"rundll32.exe",	
		st_run_command,	
		NULL,	
		0);

    if (i_result <= 32)
    {

          if (i_result == ERROR_FILE_NOT_FOUND)
    {
        LogError(_T("Can't open speech properties, file not found. Have you installed the Speech API yet?"));
    }   else
    
    if (i_result == ERROR_FILE_NOT_FOUND)
    {
        LogError(_T("Can't open speech properties, path not found.  Try using your control panel instead."));
    }   else
    
    {
        LogError(_T("Can't open speech properties, unknown error.  Try using your control panel instead."));
    } 

    }

#endif

    //new and imporoved way?  nah it sucks
/*
    if (!LaunchControlPanelApplet(_T("desk.cpl")))
    {
        LogError(_T("Unable to open Speech from control panel.  Perhaps you have not installed the Speech API yet?"));
    }
  */ 
}
