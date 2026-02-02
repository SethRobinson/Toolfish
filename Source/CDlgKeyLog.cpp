// CDlgKeyLog.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgKeyLog.h"
#include "CGlobals.h"
#include "..\Shared\uni.h"
#include "CKeyLog.h"
#include "file_config.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyLog dialog


CDlgKeyLog::CDlgKeyLog(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgKeyLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgKeyLog)
	m_b_log_active = FALSE;
	m_cst_file = _T("");
	m_b_prog_log = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgKeyLog::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgKeyLog)
	DDX_Check(pDX, IDC_B_KEY_LOG_ACTIVE, m_b_log_active);
	DDX_Text(pDX, IDC_KEY_LOG_FILE, m_cst_file);
	DDX_Check(pDX, IDC_B_PROG_LOG, m_b_prog_log);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgKeyLog, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgKeyLog)
	ON_BN_CLICKED(IDC_LOG_BROWSE, OnLogBrowse)
	ON_BN_CLICKED(IDC_VIEW_LOG, OnViewLog)
	ON_BN_CLICKED(IDC_B_KEY_LOG_ACTIVE, OnBKeyLogActive)
	ON_BN_CLICKED(IDC_DELETE_LOG, OnDeleteLog)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgKeyLog message handlers
extern CConfig glo;

BOOL CDlgKeyLog::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
 SetBitmap(_T("gui\\background.bmp"));
	//init from our settings
	 m_cst_file = glo.m_st_key_log;
     m_b_log_active = glo.m_b_key_log_enabled;
     m_b_prog_log = glo.m_b_log_programs_run;

     UpdateData(D_TO_WINDOW);
     GrayOutStuff();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKeyLog::OnLogBrowse() 
{
 	UpdateData(D_TO_VAR);
  //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 _T(".txt"), //default file extension
		 _T(""), OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 _T("Text file (*.txt)|*.txt|All files (*.*)|*.*|"),//file filter
		 AfxGetApp()->m_pMainWnd);
		TCHAR st_temp[MAX_PATH];
	 getdir(st_temp);

	 cfd.m_ofn.lpstrInitialDir = st_temp;
		 
	 if(cfd.DoModal() == IDOK)
		 {
	       m_cst_file =  cfd.GetPathName();

			UpdateData(D_TO_WINDOW);
		 } else
		 {
		 //  log_msg("Did not load.");
		 }

	
}

void CDlgKeyLog::OnViewLog() 
{
    UpdateData(D_TO_VAR);
    // view log file
   	g_key_log.FlushBuffer();
    
    
    if (!exist(uni(m_cst_file).to_st()))
    {
        //file doesn't exist
        MessageBox(_T("The file doesn't exist.\n\nNothing is logged or the filename is bad."),
            _T("You ain't viewing nothing."), NULL);
        return;
        
    }
    int result = (int)ShellExecute(NULL,_T("open"),m_cst_file, NULL,NULL, SW_SHOWDEFAULT  );
    
    
    if ( (result < 32) && (result != 2))
    {
        //big fat error.
        MessageBox(_T("Windows doesn't know how to display this file.\n\nTry")
            _T(" changing the filename of your log to something with a .txt extension.\n"),
            _T("You ain't viewing nothing."), NULL);
        
    }
    
}
void CDlgKeyLog::GrayOutStuff()
{
  UpdateData(D_TO_VAR);
  ((CWnd*)GetDlgItem(IDC_KEY_LOG_FILE))->EnableWindow(m_b_log_active);
  ((CWnd*)GetDlgItem(IDC_LOG_BROWSE))->EnableWindow(m_b_log_active);
 
}


void CDlgKeyLog::OnOK() 
{
     UpdateData(D_TO_VAR);
     wcscpy(glo.m_st_key_log, uni(m_cst_file).us_data);
     glo.m_b_key_log_enabled = m_b_log_active != 0;
     glo.m_b_log_programs_run = m_b_prog_log != 0;
     
     CBkDialogST::OnOK();
}

void CDlgKeyLog::OnBKeyLogActive() 
{

    GrayOutStuff();
	
}

void CDlgKeyLog::OnDeleteLog() 
{
  UpdateData(D_TO_VAR);
   	// TODO: Add your control notification handler code here
  
	if (MessageBox(_T("Are you sure you wish to delete your log file?"), _T("Delete Verification"),
		MB_OKCANCEL) == IDOK)
	{
	 _tunlink(m_cst_file);
	}
	
}

HBRUSH CDlgKeyLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
