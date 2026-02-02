// CDlgWizardAtomic.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgWizardAtomic.h"
#include "CDlgActionAtomic.h"
#include "..\Shared\all.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\uni.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CTrigger.h"
#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int C_ATOMIC_SIX_HOURS = 0;
const int C_ATOMIC_ONCE_A_DAY = 1;
const int C_ATOMIC_ONCE_A_WEEK = 2;
const int C_ATOMIC_ONCE_A_MONTH = 3;

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardAtomic dialog


CDlgWizardAtomic::CDlgWizardAtomic(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizardAtomic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardAtomic)
	m_cst_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgWizardAtomic::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardAtomic)
	DDX_Control(pDX, IDC_COMBO_TIME, m_combo_time);
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_CBString(pDX, IDC_COMBO, m_cst_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizardAtomic, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizardAtomic)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardAtomic message handlers

HBRUSH CDlgWizardAtomic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
            ||          (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
            ||          (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
            ||          (pWnd->GetDlgCtrlID() == IDC_GROUPBOX3)
  
          
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

BOOL CDlgWizardAtomic::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
	SetBitmap(_T("gui\\background.bmp"));
    InitAtomicComboBox(m_combo);

    m_combo.SelectString(0, _T("time-b.nist.gov"));
   
    m_combo_time.ResetContent();
    m_combo_time.AddString(_T("every six hours"));
    m_combo_time.AddString(_T("once a day"));
    m_combo_time.AddString(_T("once a week"));
    m_combo_time.AddString(_T("once a month"));

    m_combo_time.SetCurSel(C_ATOMIC_ONCE_A_DAY);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWizardAtomic::OnOK() 
{
 	//let's actually create the event

   	// TODO: Add extra validation here
  UpdateData(D_TO_VAR);

  //let's actually add the event.
     int i_event_id = app_glo.GetEventList()->AddEvent();
    CEvent *p_event = app_glo.GetEventList()->GetEvent(i_event_id);
    //save it so the filename will ve reserved in case another event is created
    app_glo.GetEventList()->SaveEvent(i_event_id);

  

      //first add the timer event
    int i_time_trig = p_event->AddTrigger();
    CTrigger *p_trig = p_event->GetTrigger(i_time_trig);
    p_trig->SetTriggerType(C_TRIGGER_TIME);
    //set timer type and time...
     //set it up to repeat
    p_trig->SetRescheduleActive(true);

    //setup the correct timing system

    if (m_combo_time.GetCurSel() == C_ATOMIC_SIX_HOURS)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_HOURS);
           p_trig->SetRescheduleRaw(6);
    }   else
    if (m_combo_time.GetCurSel() == C_ATOMIC_ONCE_A_DAY)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(1);
    }   else
    if (m_combo_time.GetCurSel() == C_ATOMIC_ONCE_A_WEEK)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(7);
    }   else
    if (m_combo_time.GetCurSel() == C_ATOMIC_ONCE_A_MONTH)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(28);
    }   else
    {
        LogError(_T("Unknown dropdown type for the timing."));
        p_trig->SetRescheduleActive(false);
    }


    //great, now let's add the actual action.
     int i_action_id = p_event->AddAction();
     CAction *p_action = p_event->GetAction(i_action_id);
     p_action->SetActionType(C_ACTION_ATOMIC);
     //set the server
     p_action->SetEmailServer(uni(m_cst_text).us_data);
    
     app_glo.GetEventList()->GetEvent(i_event_id)->SetCategoryName(_T("Events\\Atomic Clock Adjustments\\"));
      app_glo.GetEventList()->SaveEvent(i_event_id);


	CBkDialogST::OnOK();
}
