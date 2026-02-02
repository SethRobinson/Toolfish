// CDlgActionSmartMute.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionSmartMute.h"
 #include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSmartMute dialog


CDlgActionSmartMute::CDlgActionSmartMute(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionSmartMute::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionSmartMute)
	//}}AFX_DATA_INIT
}


void CDlgActionSmartMute::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionSmartMute)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionSmartMute, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionSmartMute)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSmartMute message handlers

HBRUSH CDlgActionSmartMute::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgActionSmartMute::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
 SetBitmap(_T("gui\\background.bmp"));
  m_combo.SetCurSel(m_p_action->GetTTS());
  	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionSmartMute::OnOK() 
{
		m_p_action->SetTTS(m_combo.GetCurSel() != 0);

	CBkDialogST::OnOK();
}
