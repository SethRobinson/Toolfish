// CDlgActionMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionMonitor.h"
#include "..\Shared\uni.h"
#include "CDlgOkCancel.h"
#include "CDlgTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionMonitor dialog


CDlgActionMonitor::CDlgActionMonitor(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionMonitor)
	m_b_error = FALSE;
	m_cst_filename = _T("");
	m_i_port = 0;
	m_i_size = 0;
	m_b_timeout = FALSE;
	m_b_webpage = FALSE;
	m_b_size = FALSE;
	m_b_reverse = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgActionMonitor::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionMonitor)
	DDX_Check(pDX, IDC_ERROR, m_b_error);
	DDX_Text(pDX, IDC_FILENAME, m_cst_filename);
	DDX_Text(pDX, IDC_PORT, m_i_port);
	DDX_Text(pDX, IDC_SIZE_BYTES, m_i_size);
	DDX_Check(pDX, IDC_TIMEOUT, m_b_timeout);
	DDX_Check(pDX, IDC_WEBPAGE, m_b_webpage);
	DDX_Check(pDX, IDC_SIZE_CONTENT, m_b_size);
	DDX_Check(pDX, IDC_REVERSE, m_b_reverse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionMonitor, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionMonitor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_WEBPAGE, OnWebpage)
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionMonitor message handlers

HBRUSH CDlgActionMonitor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
            ||            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
            ||            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX3)
 


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

BOOL CDlgActionMonitor::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
  SetBitmap(_T("gui\\background.bmp"));
	m_cst_filename = m_p_action->GetFilename();

    if (m_cst_filename.GetLength() == 0)
    {
       m_cst_filename = L"www.rtsoft.com";
    }
	

    m_b_timeout = m_p_action->GetMonitorTimeout();
    m_b_size = m_p_action->GetMonitorSize();
    m_i_size = m_p_action->GetMonitorSizeBytes();
    m_i_port = m_p_action->GetPort();
    m_b_error = m_p_action->GetMonitorError();
    m_b_webpage = m_p_action->GetWebPage();
    m_b_reverse = m_p_action->GetMonitorReverse();

    SetGrayStuff();
    UpdateStats();
    
    UpdateData(D_TO_WINDOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionMonitor::SetGrayStuff()
{
 ((CButton*)GetDlgItem(IDC_SIZE_CONTENT))->EnableWindow(m_b_webpage);
 ((CButton*)GetDlgItem(IDC_SIZE_BYTES))->EnableWindow(m_b_webpage);
 ((CButton*)GetDlgItem(IDC_ERROR))->EnableWindow(m_b_webpage);

}

void CDlgActionMonitor::UpdateStats()
{
    CString cst_stats;
    cst_stats.Format(_T("Out of %d checks the site failed %d times. (%d %% uptime)"),
        m_p_action->GetMonitorSamples(), 
        m_p_action->GetMonitorFails(),
        int(m_p_action->GetUptime()*100.0f));
    
    
    ((CStatic*)GetDlgItem(IDC_TEXT1))->SetWindowText(cst_stats);

    //now set the second line of text

    cst_stats.Format(_T("Average ping is %d.  Lowest was %d, highest was %d."),
        m_p_action->GetAveragePing(), m_p_action->GetLowPing(), m_p_action->GetHighPing());

    ((CStatic*)GetDlgItem(IDC_TEXT2))->SetWindowText(cst_stats);

}


void CDlgActionMonitor::OnOK() 
{
    UpdateData(D_TO_VAR);
    m_p_action->SetFilename(uni(m_cst_filename).us_data);
    m_p_action->SetMonitorTimeout(m_b_timeout != 0);
    m_p_action->SetMonitorError(m_b_error != 0);
    m_p_action->SetMonitorSize(m_b_size != 0);
    m_p_action->SetMonitorSizeBytes(m_i_size);
    m_p_action->SetPort(m_i_port);
    m_p_action->SetWebPage(m_b_webpage != 0);
    m_p_action->SetMonitorReverse(m_b_reverse != 0);
 	CBkDialogST::OnOK();
}

void CDlgActionMonitor::OnWebpage() 
{
	// TODO: Add your control notification handler code here
    UpdateData(D_TO_VAR);
    SetGrayStuff();
}

void CDlgActionMonitor::OnReset() 
{
	// TODO: Add your control notification handler code here
	m_p_action->ClearMonitorStats();
    UpdateStats();
    this->InvalidateRect(NULL);
}
