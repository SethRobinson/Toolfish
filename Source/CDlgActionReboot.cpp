// CDlgActionReboot.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionReboot.h"
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
// CDlgActionReboot dialog


CDlgActionReboot::CDlgActionReboot(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionReboot::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionReboot)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgActionReboot::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionReboot)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionReboot, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionReboot)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionReboot message handlers

HBRUSH CDlgActionReboot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgActionReboot::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
 SetBitmap(_T("gui\\background.bmp"));
 m_combo.ResetContent();
 m_combo.AddString(_T("Logoff current user"));
 m_combo.AddString(_T("Shutdown computer"));
 m_combo.AddString(_T("Shutdown and reboot computer"));
 m_combo.SetCurSel(m_p_action->GetRebootType());
 
  UpdateData(D_TO_WINDOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionReboot::OnOK() 
{
UpdateData(D_TO_VAR);

m_p_action->SetRebootType(m_combo.GetCurSel());

	CBkDialogST::OnOK();
}
