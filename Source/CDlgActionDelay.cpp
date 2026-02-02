// CDlgActionDelay.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionDelay.h"
#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionDelay dialog


CDlgActionDelay::CDlgActionDelay(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionDelay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionDelay)
	m_i_ms = 0;
	//}}AFX_DATA_INIT
}


void CDlgActionDelay::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionDelay)
	DDX_Text(pDX, IDC_MS, m_i_ms);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionDelay, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionDelay)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionDelay message handlers

HBRUSH CDlgActionDelay::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
          /*
          if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
              pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }
            */
        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
     || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
      || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
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
    //    hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
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

BOOL CDlgActionDelay::OnInitDialog() 
{
    CBkDialogST::OnInitDialog();
    
    SetBitmap(_T("gui\\background.bmp"));
    m_i_ms = m_p_action->GetDelay();
    UpdateData(D_TO_WINDOW);
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionDelay::OnOK() 
{
	UpdateData(D_TO_VAR);
    m_p_action->SetDelay(m_i_ms);

	CBkDialogST::OnOK();
}
