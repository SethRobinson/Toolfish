// CDlgTriggerInactivity.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgTriggerInactivity.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"
#include "CTrigger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerInactivity dialog


CDlgTriggerInactivity::CDlgTriggerInactivity(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTriggerInactivity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTriggerInactivity)
	m_cst_edit = _T("");
	//}}AFX_DATA_INIT
}


void CDlgTriggerInactivity::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTriggerInactivity)
	DDX_Text(pDX, IDC_EDIT, m_cst_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTriggerInactivity, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTriggerInactivity)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerInactivity message handlers

HBRUSH CDlgTriggerInactivity::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgTriggerInactivity::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
   	
    m_cst_edit.Format(_T("%d"), m_p_trigger->GetRescheduleRaw());

    //set the radio controls

    CButton* p_button;

    if (m_p_trigger->GetRescheduleType() == C_TRIGGER_TIME_SECONDS)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_SECONDS);  
    }
    else

    if (m_p_trigger->GetRescheduleType() == C_TRIGGER_TIME_MINUTES)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_MINUTES);  
    }  
    else
    {
        //default if nothing is checked which should never happen
        p_button = (CButton*)GetDlgItem(IDC_RADIO_MINUTES); 
    }
 
        
        p_button->SetCheck(true);
   
    //also the other ones

    if (m_p_trigger->m_b_check_for_activity )
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_HAS);  
    } else
    {

      p_button = (CButton*)GetDlgItem(IDC_RADIO_HAS_NOT);  
    }
    
    p_button->SetCheck(true);

    UpdateData(D_TO_WINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTriggerInactivity::OnOK() 
{
   
    UpdateData(D_TO_VAR);

    if (((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->GetCheck()) 
      m_p_trigger->SetRescheduleType(C_TRIGGER_TIME_SECONDS);
    if (((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->GetCheck()) 
      m_p_trigger->SetRescheduleType(C_TRIGGER_TIME_MINUTES);

  
      if (((CButton*)GetDlgItem(IDC_RADIO_HAS))->GetCheck()) 
      m_p_trigger->m_b_check_for_activity = true; else
      m_p_trigger->m_b_check_for_activity = false;

    m_p_trigger->SetRescheduleRaw(_tcstol(m_cst_edit, NULL, 10));
	
	CBkDialogST::OnOK();
}
