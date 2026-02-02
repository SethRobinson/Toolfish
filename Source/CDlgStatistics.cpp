// CDlgStatistics.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgStatistics.h"
#include "CGlobals.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistics dialog


CDlgStatistics::CDlgStatistics(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgStatistics::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStatistics)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgStatistics::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStatistics)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStatistics, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgStatistics)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistics message handlers

HBRUSH CDlgStatistics::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
           ||(pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
           ||            (pWnd->GetDlgCtrlID() == IDC_SLIDER)
 
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
        //hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
            hbr=(HBRUSH) m_hbrush;
        
        return hbr;
        break;
      
    }
	return hbr;

}

BOOL CDlgStatistics::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
	SetBitmap(_T("gui\\background.bmp"));

    UpdateStats();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStatistics::UpdateStats()
{

    //do todays
    
    CString cst_stats;
    CString cst_final;


    cst_stats.Format(_T("You've tapped %s keys today.\r\n\r\n"),
        ulong_to_cash(glo.m_stats_today.GetKeys()));
    
    cst_final += cst_stats;

      cst_stats.Format(_T("You've moved your mouse about %s and spun your wheel %s notches today.\r\n\r\n"),
        glo.m_stats_today.GetMouseAsText(), glo.m_stats_today.GetMouseButtonAsText(C_MOUSE_WHEEL));
    cst_final += cst_stats;

    cst_stats.Format(_T("Mouse buttons: Left: %s, Right: %s, Middle: %s"),
        glo.m_stats_today.GetMouseButtonAsText(C_MOUSE_LEFT),
       glo.m_stats_today.GetMouseButtonAsText(C_MOUSE_RIGHT) ,
       glo.m_stats_today.GetMouseButtonAsText(C_MOUSE_MIDDLE));

        cst_final += cst_stats;

    
    ((CStatic*)GetDlgItem(IDC_TODAY_BUTTONS))->SetWindowText(cst_final);


  cst_final = _T("");

  cst_stats.Format(_T("You've tapped %s keys total.\r\n\r\n"),
        ulong_to_cash(glo.m_stats_total.GetKeys()));
   cst_final += cst_stats;

    cst_stats.Format(_T("You've moved your mouse about %s and spun your wheel %s notches total.\r\n\r\n"),
        glo.m_stats_total.GetMouseAsText(), glo.m_stats_total.GetMouseButtonAsText(C_MOUSE_WHEEL));
   cst_final += cst_stats;


     cst_stats.Format(_T("Mouse buttons: Left: %s, Right: %s, Middle: %s"),
        glo.m_stats_total.GetMouseButtonAsText(C_MOUSE_LEFT),
       glo.m_stats_total.GetMouseButtonAsText(C_MOUSE_RIGHT) ,
       glo.m_stats_total.GetMouseButtonAsText(C_MOUSE_MIDDLE));
      cst_final += cst_stats;


     
     
     ((CStatic*)GetDlgItem(IDC_TOTAL_BUTTONS))->SetWindowText(cst_final);




}

void CDlgStatistics::OnReset() 
{

    glo.m_stats_total.Reset();

    UpdateStats();
    InvalidateRect(NULL);

}
