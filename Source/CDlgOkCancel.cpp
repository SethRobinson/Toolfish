// CDlgOkCancel.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgOkCancel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgOkCancel dialog


DlgOkCancel::DlgOkCancel(CWnd* pParent /*=NULL*/)
	: CBkDialogST(DlgOkCancel::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgOkCancel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgOkCancel::DoDataExchange(CDataExchange* pDX)
{
    CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgOkCancel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgOkCancel, CBkDialogST)
	//{{AFX_MSG_MAP(DlgOkCancel)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgOkCancel message handlers

BOOL DlgOkCancel::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	 SetBitmap(_T("gui\\background.bmp"));
	
	// TODO: Add extra initialization here
    this->SetWindowText(m_cst_title);
	
    if (m_cst_dialog.Compare(_T("")) == 0) m_cst_dialog = _T("Are you sure you wish to quit?  To continue to run Toolfish in the background click Cancel, then the Hide To Tray button.");
    
    ((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(m_cst_dialog);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgOkCancel::SetTitle(TCHAR * p_message)
{
    //save this for later
    m_cst_title = p_message;
    
}

void DlgOkCancel::SetDialogText(TCHAR * p_message)
{
    //save this for later
    m_cst_dialog = p_message;
    
}


void DlgOkCancel::OnOK() 
{
	// TODO: Add extra validation here
	
	CBkDialogST::OnOK();
}

HBRUSH DlgOkCancel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
