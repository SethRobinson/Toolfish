// CDlgWizardMonitor.cpp : implementation file
//


#include "stdafx.h"
#include "source.h"
#include "CDlgWizardMonitor.h"
#include "..\Shared\uni.h"
#include "CAction.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CDlgAlert.h"
#include "CSMPTSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardMonitor dialog


CDlgWizardMonitor::CDlgWizardMonitor(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizardMonitor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardMonitor)
	m_b_email = FALSE;
	m_cst_email_from = _T("");
	m_cst_email_to = _T("");
	m_cst_filename = _T("");
	m_b_popup = FALSE;
	m_i_port = 0;
	m_b_quiet = FALSE;
	m_b_say = FALSE;
	m_b_webpage = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgWizardMonitor::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardMonitor)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Check(pDX, IDC_EMAIL, m_b_email);
	DDX_Text(pDX, IDC_EMAIL_FROM, m_cst_email_from);
	DDX_Text(pDX, IDC_EMAIL_TO, m_cst_email_to);
	DDX_Text(pDX, IDC_FILENAME, m_cst_filename);
	DDX_Check(pDX, IDC_POPUP, m_b_popup);
	DDX_Text(pDX, IDC_PORT, m_i_port);
	DDX_Check(pDX, IDC_QUIET, m_b_quiet);
	DDX_Check(pDX, IDC_SAY, m_b_say);
	DDX_Check(pDX, IDC_WEBPAGE, m_b_webpage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizardMonitor, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizardMonitor)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SMTP, OnSmtp)
	ON_BN_CLICKED(IDC_EMAIL, OnEmail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardMonitor message handlers

HBRUSH CDlgWizardMonitor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgWizardMonitor::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
   SetBitmap(_T("gui\\background.bmp"));
     m_cst_filename = L"www.rtsoft.com";
     m_i_port = 80;
     m_b_webpage = false;
#ifdef _UNICODE
     m_cst_email_from = glo.m_st_email;
#else
     m_cst_email_from = uni(glo.m_st_email).to_st();
#endif
 m_cst_email_to = "";
     m_b_say = true;
     m_combo.SetCurSel(3); //3 minute check
     
     GrayOutStuff();
     UpdateData(D_TO_WINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWizardMonitor::OnOK() 
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
           p_trig->SetRescheduleRaw(5);
    }   else
    if (m_combo.GetCurSel() == 2)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);
           p_trig->SetRescheduleRaw(30);
    }   else
    if (m_combo.GetCurSel() == 3)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_HOURS);
           p_trig->SetRescheduleRaw(1);
    }   else
   
    if (m_combo.GetCurSel() == 4)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
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
     p_action->SetActionType(C_ACTION_MONITOR);
     p_event->SetQuietMode(m_b_quiet != 0);

     p_action->SetFilename(uni(m_cst_filename).us_data);
     p_action->SetPort(m_i_port);
     p_action->SetWebPage(m_b_webpage != 0);
     p_action->SetMonitorError(true);
     p_action->SetMonitorTimeout(true);

     //now add any special actions to be done
     if (m_b_say || m_b_popup || m_b_email)
     {
         
         
         i_action_id = p_event->AddAction();
         p_action = p_event->GetAction(i_action_id);
         p_action->SetActionType(C_ACTION_MESSAGE);
         p_action->CopyToBuffer(L"Uh oh, $LAST_COMPARE_URL$ is not responding.");
         p_action->m_b_say = m_b_say != 0;
         p_action->m_b_popup = m_b_popup != 0;
        
         if ( p_action->m_b_email = (m_b_email!=0)) //on purpose, I'm setting the value here
         {
            p_action->SetToEmail(uni(m_cst_email_to).us_data); 
            p_action->SetReturnEmail(uni(m_cst_email_from).us_data); 
            p_action->SetEmailSubject(L"Toolfish Site Monitor Notification"); 

         }

         
     }

     app_glo.GetEventList()->SaveEvent(i_event_id);

      CDlgAlert dlg;
      TCHAR *p_text = _T("Your new site monitor event has been set up!\r\n\r\nRemember, to access more detailed options or view statistics use the Event Manager, you'll find this event in the Server Monitor folder.");
      dlg.SetWindowCaption(_T("Email Check Event Created!"));

      dlg.StoreTextPointerForLater(p_text);
	  dlg.DoModal();



	
	CBkDialogST::OnOK();
}

void CDlgWizardMonitor::OnSmtp() 
{
	 CSMPTSetup dlg;
    dlg.DoModal();

}

void CDlgWizardMonitor::GrayOutStuff()
{
 ((CButton*)GetDlgItem(IDC_EMAIL_TO))->EnableWindow(m_b_email);
 ((CButton*)GetDlgItem(IDC_EMAIL_FROM))->EnableWindow(m_b_email);
 


}



void CDlgWizardMonitor::OnEmail() 
{
	UpdateData(D_TO_VAR);
    GrayOutStuff();
	InvalidateRect(NULL);
   
}
