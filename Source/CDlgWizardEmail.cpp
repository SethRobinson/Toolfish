// CDlgWizardEmail.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgWizardEmail.h"
#include "..\Shared\all.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\uni.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CTrigger.h"
#include "CAction.h"
#include "CDlgAlert.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardEmail dialog


CDlgWizardEmail::CDlgWizardEmail(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizardEmail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardEmail)
	m_cst_logon = _T("");
	m_cst_password = _T("");
	m_cst_server = _T("");
	m_b_say_count = FALSE;
	m_b_say_from = FALSE;
	m_b_say_subject = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgWizardEmail::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardEmail)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Text(pDX, IDC_NAME, m_cst_logon);
	DDX_Text(pDX, IDC_PASSWORD, m_cst_password);
	DDX_Text(pDX, IDC_SERVER, m_cst_server);
	DDX_Check(pDX, IDC_SAY_COUNT, m_b_say_count);
	DDX_Check(pDX, IDC_SAY_FROM, m_b_say_from);
	DDX_Check(pDX, IDC_SAY_SUBJECT, m_b_say_subject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizardEmail, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizardEmail)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardEmail message handlers

HBRUSH CDlgWizardEmail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgWizardEmail::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
	SetBitmap(_T("gui\\background.bmp"));
  	//set defaults

    m_b_say_count = true;
    m_b_say_from = true;
    m_b_say_subject = true;

    m_combo.SetCurSel(1); //3 minute check
    m_cst_server = _T("mail.yourserver.com");


    
    UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWizardEmail::OnOK() 
{
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

    if (m_combo.GetCurSel() == 0)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);
           p_trig->SetRescheduleRaw(1);
    }   else
    if (m_combo.GetCurSel() == 1)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);
           p_trig->SetRescheduleRaw(3);
    }   else
    if (m_combo.GetCurSel() == 2)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);
           p_trig->SetRescheduleRaw(5);
    }   else
    if (m_combo.GetCurSel() == 3)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_HOURS);
           p_trig->SetRescheduleRaw(1);
    }   else
   
    {
        LogError(_T("Unknown dropdown type for the timing."));
          p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);
          p_trig->SetRescheduleRaw(3);
   }


    //great, now let's add the actual action.
     int i_action_id = p_event->AddAction();
     CAction *p_action = p_event->GetAction(i_action_id);
     p_action->SetActionType(C_ACTION_EMAIL);
     //set the server
     p_action->SetEmailServer(uni(m_cst_server).us_data);
     p_action->SetPassword(uni(m_cst_password).us_data);
     p_action->SetLogon(uni(m_cst_logon).us_data);
    
    p_action->m_b_say_count = m_b_say_count != 0;
    p_action->m_b_say_from = m_b_say_from != 0;
    p_action->m_b_say_subject = m_b_say_subject != 0;

    p_event->SetQuietMode(true);


   //  app_glo.GetEventList()->GetEvent(i_event_id)->SetCategoryName(_T("Events\\Atomic Clock Adjustments\\"));
      app_glo.GetEventList()->SaveEvent(i_event_id);


    //show additional info to them

      CDlgAlert dlg;
      TCHAR *p_text = _T("The email check has been set up.\r\n\r\n'Quiet Mode' has been activated for this event, you will only see messages when new email is actually found. (it helps keep the log clean)\r\n\r\nRemember, to access more detailed options or fix any mistakes in this event use the Event Manager.");
      dlg.SetWindowCaption(_T("Email Check Event Created!"));

      dlg.StoreTextPointerForLater(p_text);
	  dlg.DoModal();

	CBkDialogST::OnOK();
}
