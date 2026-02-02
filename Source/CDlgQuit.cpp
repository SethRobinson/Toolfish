// CDlgQuit.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgQuit.h"
 #include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQuit dialog


CDlgQuit::CDlgQuit(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgQuit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQuit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgQuit::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQuit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuit, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgQuit)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDHIDE, OnHide)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDCANCEL, &CDlgQuit::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQuit message handlers

HBRUSH CDlgQuit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
        hbr=(HBRUSH) (m_hbrush);
        return hbr;
        break;
      
    }

	return hbr;
}

BOOL CDlgQuit::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
   SetBitmap(_T("gui\\background.bmp"));
  	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgQuit::OnHide() 
{
	// TODO: Add your control notification handler code here
    ::PostMessage(app_glo.GetHWND(), WM_HIDE, 0,0);
     OnCancel();
}


void CDlgQuit::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
}
