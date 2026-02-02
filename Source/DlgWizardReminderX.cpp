// DlgWizardReminderX.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CGlobals.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CTrigger.h"
#include "CAction.h"
#include "std_all.h"

#include "DlgWizardReminderX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardReminderX dialog


CDlgWizardReminderX::CDlgWizardReminderX(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizardReminderX::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardReminderX)
	m_b_popup = FALSE;
	m_b_say = FALSE;
	m_cst_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgWizardReminderX::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardReminderX)
	DDX_Check(pDX, IDC_B_POPUP, m_b_popup);
	DDX_Check(pDX, IDC_B_SAY, m_b_say);
	DDX_Text(pDX, IDC_TEXT, m_cst_text);
	//}}AFX_DATA_MAP
}
     

int CDlgWizardReminderX::Init(int i_time)
{
    m_delay_seconds = i_time;
    
    return DoModal();
}

BEGIN_MESSAGE_MAP(CDlgWizardReminderX, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizardReminderX)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_HEAR, OnHear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardReminderX message handlers

BOOL CDlgWizardReminderX::OnInitDialog() 
{
    CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
	
	
    // TODO: Add extra initialization here
    CTime time_temp(m_delay_seconds);
    
    CString st_temp;
    
    TCHAR *p_time = ShowTimePeriod(m_delay_seconds);

    st_temp.Format(_T("You will be reminded in %s."),p_time);
    SAFE_DELETE_ARRAY(p_time);

    ((CStatic*)GetDlgItem(IDC_DESCRIPTION))->SetWindowText(st_temp);
    m_cst_text.Format(_T("Reminder triggered!"));
  	
    m_b_say = true;
    m_b_popup = true;
    UpdateData(D_TO_WINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgWizardReminderX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgWizardReminderX::OnOK() 
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
   
    CTime timer = CTime::GetCurrentTime() + m_delay_seconds;
    
    p_trig->SetTime(&timer);

    //add what happens when it's scheduled
     int i_action_id = p_event->AddAction();
     CAction *p_action = p_event->GetAction(i_action_id);
      p_action->SetActionType(C_ACTION_MESSAGE);

      p_action->CopyToBuffer(uni(m_cst_text).us_data);

      p_action->m_b_log = true;
      p_action->m_b_say = m_b_say != 0;
      p_action->m_b_popup = m_b_popup != 0;

     //and finally, cause this to self distruct after being run.
    app_glo.GetEventList()->GetEvent(i_event_id)->SetAutoDeleteAfterRunning(true);
    app_glo.GetEventList()->GetEvent(i_event_id)->SetCategoryName(_T("Events\\Reminders\\"));
    app_glo.GetEventList()->SaveEvent(i_event_id);

	CBkDialogST::OnOK();
}

void CDlgWizardReminderX::OnHear() 
{
    UpdateData(D_TO_VAR);
    
    if (app_glo.GetSpeech())
    {
        //SAPI won't work in a thread, needs the message pump for messaging
        //Let's send it to our main window to play
        //global say will make a copy and send it
        glo.m_voice.SayTest(m_cst_text);
    } else
    {
        LogError(_T("Microsoft SAPI has been disabled from the options menu, can't use text to speech."));
    }
	
}
