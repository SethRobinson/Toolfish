// CDlgWizardRecurrentReminder.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgWizardRecurrentReminder.h"
#include "DlgWizardOTReminder.h"
#include "..\Shared\all.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\uni.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CTrigger.h"
#include "CAction.h"

const int C_REMINDER_HOURLY = 0;
const int C_REMINDER_DAILY = 1;
const int C_REMINDER_WEEKLY = 2;
const int C_REMINDER_MONTHLY = 3;
const int C_REMINDER_YEARLY = 4;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardRecurrentReminder dialog


CDlgWizardRecurrentReminder::CDlgWizardRecurrentReminder(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizardRecurrentReminder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardRecurrentReminder)
	m_b_popup = FALSE;
	m_b_say = FALSE;
	m_b_0 = FALSE;
	m_b_1 = FALSE;
	m_b_2 = FALSE;
	m_b_3 = FALSE;
	m_b_4 = FALSE;
	m_b_5 = FALSE;
	m_b_6 = FALSE;
	m_cst_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgWizardRecurrentReminder::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardRecurrentReminder)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Control(pDX, IDC_PIC, m_but_pic);
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_date);
	DDX_Check(pDX, IDC_B_POPUP, m_b_popup);
	DDX_Check(pDX, IDC_B_SAY, m_b_say);
	DDX_Check(pDX, IDC_DAY0, m_b_0);
	DDX_Check(pDX, IDC_DAY1, m_b_1);
	DDX_Check(pDX, IDC_DAY2, m_b_2);
	DDX_Check(pDX, IDC_DAY3, m_b_3);
	DDX_Check(pDX, IDC_DAY4, m_b_4);
	DDX_Check(pDX, IDC_DAY5, m_b_5);
	DDX_Check(pDX, IDC_DAY6, m_b_6);
	DDX_Text(pDX, IDC_TEXT, m_cst_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizardRecurrentReminder, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizardRecurrentReminder)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_HEAR, OnHear)
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardRecurrentReminder message handlers

HBRUSH CDlgWizardRecurrentReminder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgWizardRecurrentReminder::OnHear() 
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

void CDlgWizardRecurrentReminder::OnOK() 
{
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

    if (m_combo.GetCurSel() == C_REMINDER_HOURLY)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_HOURS);
           p_trig->SetRescheduleRaw(1);
    }   else
    if (m_combo.GetCurSel() == C_REMINDER_DAILY)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(1);
    }   else
    if (m_combo.GetCurSel() == C_REMINDER_WEEKLY)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(7);
    }   else
    if (m_combo.GetCurSel() == C_REMINDER_MONTHLY)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(28);
    }   else
   if (m_combo.GetCurSel() == C_REMINDER_YEARLY)
    {
           p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);
           p_trig->SetRescheduleRaw(365);
    }   else


    {
        LogError(_T("Unknown dropdown type for the timing."));
        p_trig->SetRescheduleActive(false);
    }
    
        SYSTEMTIME sys_date;  
    m_date.GetCurSel(&sys_date);
     sys_date.wHour = sys_date.wMinute = sys_date.wSecond = sys_date.wMilliseconds =  0;

    CTime time2_temp;
  
    m_time.GetTime(time2_temp);
   
    CTime time_temp(sys_date);

  CTime time_final(time_temp.GetYear(), time_temp.GetMonth(), time_temp.GetDay(),
       time2_temp.GetHour(), time2_temp.GetMinute(), time2_temp.GetSecond());
    
     p_trig->SetTime(&time_final);

 
     if (m_combo.GetCurSel() == C_REMINDER_DAILY)
     {
         //also add a day filter trigger
        int i_day_filter_trig = p_event->AddTrigger();
        CTrigger *p_day_trig = p_event->GetTrigger(i_day_filter_trig);
        p_day_trig->SetTriggerType(C_TRIGGER_DAY_FILTER);
  
        p_day_trig->m_b_a_day_active[0] = m_b_0 != 0;
        p_day_trig->m_b_a_day_active[1] = m_b_1 != 0;
        p_day_trig->m_b_a_day_active[2] = m_b_2 != 0;
        p_day_trig->m_b_a_day_active[3] = m_b_3 != 0;
        p_day_trig->m_b_a_day_active[4] = m_b_4 != 0;
        p_day_trig->m_b_a_day_active[5] = m_b_5 != 0;
        p_day_trig->m_b_a_day_active[6] = m_b_6 != 0;



     }



    //add what happens when it's scheduled
     int i_action_id = p_event->AddAction();
     CAction *p_action = p_event->GetAction(i_action_id);
      p_action->SetActionType(C_ACTION_MESSAGE);
#ifdef _UNICODE
      p_action->CopyToBuffer(m_cst_text);
#else
      p_action->CopyToBuffer(uni(m_cst_text).us_data);

#endif
      
      p_action->m_b_log = true;
      p_action->m_b_say = m_b_say != 0;
      p_action->m_b_popup = m_b_popup != 0;


     //and finally, cause this to self distruct after being run.
   //  app_glo.GetEventList()->GetEvent(i_event_id)->SetAutoDeleteAfterRunning(true);
     
    app_glo.GetEventList()->GetEvent(i_event_id)->SetCategoryName(_T("Events\\Reminders\\"));
    
    app_glo.GetEventList()->SaveEvent(i_event_id);


	
	CBkDialogST::OnOK();
}

BOOL CDlgWizardRecurrentReminder::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
   SetBitmap(_T("gui\\background.bmp"));
	
    m_but_pic.SetTooltipText(_T("It's easy and fast to add a reminder!"));
   m_but_pic.DrawTransparent();
   m_but_pic.SetBitmaps(_T("gui\\but_wizard.bmp"), RGB(255,255,255), 0,0);
   //m_but_pic.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
  // m_but_pic.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
   m_but_pic.SizeToContent();
   m_but_pic.DrawBorder(false, false);

   //set day checkbox defaults
   m_b_0 = true;
   m_b_1 = true;
   m_b_2 = true;
   m_b_3 = true;
   m_b_4 = true;
   m_b_5 = true;
   m_b_6 = true;
   
   m_b_popup = false;
   m_b_say = true;

   int i= random_range(1, 100);

   if (i == 7) m_cst_text = _T("Drink a gallon of water now, it's good for you or something."); else
  if (i == 8) m_cst_text = _T("Lucid Dream Training:  Are you dreaming right now?"); else 
   if (i == 8) m_cst_text = _T("Feed cat."); else
   {
     m_cst_text = _T("");
   }

   //setup that combo box
   InitComboBox();

  
   UpdateData(D_TO_WINDOW);
   SetGray(); //grayout stuff

	
   return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWizardRecurrentReminder::InitComboBox()
{
    m_combo.ResetContent();

    m_combo.AddString(_T("Hourly"));
    m_combo.AddString(_T("Daily"));
    m_combo.AddString(_T("Weekly"));
    m_combo.AddString(_T("Monthly"));
    m_combo.AddString(_T("Yearly"));

    //set the default
    m_combo.SetCurSel(C_REMINDER_DAILY);
}

void CDlgWizardRecurrentReminder::SetGray()
{
    bool b_active = false;
    if (m_combo.GetCurSel() == C_REMINDER_DAILY) b_active = true;

     ((CButton*)GetDlgItem(IDC_DAY0))->EnableWindow(b_active);
     ((CButton*)GetDlgItem(IDC_DAY1))->EnableWindow(b_active);
     ((CButton*)GetDlgItem(IDC_DAY2))->EnableWindow(b_active);
     ((CButton*)GetDlgItem(IDC_DAY3))->EnableWindow(b_active);
     ((CButton*)GetDlgItem(IDC_DAY4))->EnableWindow(b_active);
     ((CButton*)GetDlgItem(IDC_DAY5))->EnableWindow(b_active);
     ((CButton*)GetDlgItem(IDC_DAY6))->EnableWindow(b_active);
}

void CDlgWizardRecurrentReminder::OnSelchangeCombo() 
{
    SetGray();
}
