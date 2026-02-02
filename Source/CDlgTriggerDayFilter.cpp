// CDlgTriggerDayFilter.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgTriggerDayFilter.h"
#include "CTrigger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerDayFilter dialog


CDlgTriggerDayFilter::CDlgTriggerDayFilter(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTriggerDayFilter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTriggerDayFilter)
	m_b_0 = FALSE;
	m_b_1 = FALSE;
	m_b_2 = FALSE;
	m_b_3 = FALSE;
	m_b_4 = FALSE;
	m_b_5 = FALSE;
	m_b_6 = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTriggerDayFilter::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTriggerDayFilter)
	DDX_Check(pDX, IDC_DAY0, m_b_0);
	DDX_Check(pDX, IDC_DAY1, m_b_1);
	DDX_Check(pDX, IDC_DAY2, m_b_2);
	DDX_Check(pDX, IDC_DAY3, m_b_3);
	DDX_Check(pDX, IDC_DAY4, m_b_4);
	DDX_Check(pDX, IDC_DAY5, m_b_5);
	DDX_Check(pDX, IDC_DAY6, m_b_6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTriggerDayFilter, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTriggerDayFilter)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerDayFilter message handlers

HBRUSH CDlgTriggerDayFilter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgTriggerDayFilter::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
	// TODO: Add extra initialization here
	 SetBitmap(_T("gui\\background.bmp"));
  
     //set the defaults

     m_b_0 = m_p_trigger->m_b_a_day_active[0];
     m_b_1 = m_p_trigger->m_b_a_day_active[1];
     m_b_2 = m_p_trigger->m_b_a_day_active[2];
     m_b_3 = m_p_trigger->m_b_a_day_active[3];
     m_b_4 = m_p_trigger->m_b_a_day_active[4];
     m_b_5 = m_p_trigger->m_b_a_day_active[5];
     m_b_6 = m_p_trigger->m_b_a_day_active[6];
     

     UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTriggerDayFilter::OnOK() 
{
	// TODO: Add extra validation here
	 UpdateData(D_TO_VAR);
     m_p_trigger->m_b_a_day_active[0] = m_b_0 != 0;
     m_p_trigger->m_b_a_day_active[1] = m_b_1 != 0;
     m_p_trigger->m_b_a_day_active[2] = m_b_2 != 0;
     m_p_trigger->m_b_a_day_active[3] = m_b_3 != 0;
     m_p_trigger->m_b_a_day_active[4] = m_b_4 != 0;
     m_p_trigger->m_b_a_day_active[5] = m_b_5 != 0;
     m_p_trigger->m_b_a_day_active[6] = m_b_6 != 0;

	CBkDialogST::OnOK();
}
