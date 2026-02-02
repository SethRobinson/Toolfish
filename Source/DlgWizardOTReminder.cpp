// DlgWizardOTReminder.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "cglobals.h"
#include "DlgWizardOTReminder.h"
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

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardOTReminder dialog

CDlgWizardOTReminder::CDlgWizardOTReminder(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizardOTReminder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizardOTReminder)
	m_cst_text = _T("");
	m_b_popup = FALSE;
	m_b_say = FALSE;
	m_b_disable = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgWizardOTReminder::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizardOTReminder)
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_date);
	DDX_Control(pDX, IDC_PIC, m_but_pic);
	DDX_Text(pDX, IDC_TEXT, m_cst_text);
	DDX_Check(pDX, IDC_B_POPUP, m_b_popup);
	DDX_Check(pDX, IDC_B_SAY, m_b_say);
	DDX_Check(pDX, IDC_DISABLE, m_b_disable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizardOTReminder, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizardOTReminder)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_HEAR, OnHear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardOTReminder message handlers

HBRUSH CDlgWizardOTReminder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgWizardOTReminder::OnInitDialog() 
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
	
   m_b_popup = false;
   m_b_say = true;

   int i= random_range(1, 100);

   if (i == 1) m_cst_text = _T("Saved by the bell is on!  That zack morris is so crazy."); else
   if (i == 2) m_cst_text = _T("It's time to hatch an evil plan."); else
   if (i == 3) m_cst_text = _T("Pickup kids from school!");else
 
   if (i == 4) m_cst_text = _T("Get dressed up for your big date!"); else
   if (i == 5) m_cst_text = _T("Call Bill Gates back."); else
   if (i == 6) m_cst_text = _T("Enter reminder reason here.");else 
   if (i == 7) m_cst_text = _T("Drink a gallon of water now, it's good for you."); else
   if (i == 8) m_cst_text = _T("Lucid Dream Training:  Are you dreaming right now?");else 
   if (i == 9) m_cst_text = _T("Dentist appointment.  Prepare for pain."); else
   if (i == 10) m_cst_text = _T("Go to hospital and deliver baby."); else
   if (i == 11) m_cst_text = _T("Go to friend's lan party."); else
   {
     m_cst_text = _T("");
   }

  //if (i == 7) m_cst_text = _T("Drink a gallon of water now, it's good for you."); 
   //if (i == 8) m_cst_text = _T("Lucid Dream Training:  Are you dreaming right now?"); 
   //if (i == 8) m_cst_text = _T("Feed cat."); 

   UpdateData(D_TO_WINDOW);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWizardOTReminder::OnOK() 
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
    p_trig->SetRescheduleActive(false);
/*    
   if (((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->GetCheck()) 
     p_trig->SetRescheduleType(C_TRIGGER_TIME_SECONDS);

    if (((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->GetCheck()) 
      p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);

    if (((CButton*)GetDlgItem(IDC_RADIO_HOURS))->GetCheck()) 
     p_trig->SetRescheduleType(C_TRIGGER_TIME_HOURS);
  
    if (((CButton*)GetDlgItem(IDC_RADIO_DAYS))->GetCheck()) 
     p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);

   
    p_trig->SetRescheduleRaw(_tcstol(m_st_check_time, NULL, 10));
      */
      SYSTEMTIME sys_date;  
    m_date.GetCurSel(&sys_date);
     sys_date.wHour = sys_date.wMinute = sys_date.wSecond = sys_date.wMilliseconds =  0;

    CTime time2_temp;
  
    m_time.GetTime(time2_temp);
   
    CTime time_temp(sys_date);

  CTime time_final(time_temp.GetYear(), time_temp.GetMonth(), time_temp.GetDay(),
       time2_temp.GetHour(), time2_temp.GetMinute(), time2_temp.GetSecond());
    
     p_trig->SetTime(&time_final);

     

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
      if (!m_b_disable)
      {
      
      app_glo.GetEventList()->GetEvent(i_event_id)->SetAutoDeleteAfterRunning(true);
      }
    app_glo.GetEventList()->GetEvent(i_event_id)->SetCategoryName(_T("Events\\Reminders\\"));
    
    app_glo.GetEventList()->SaveEvent(i_event_id);



  CBkDialogST::OnOK();
}

void CDlgWizardOTReminder::OnHear() 
{
	// TODO: Add your control notification handler code here
    UpdateData(D_TO_VAR);
  
   /*
     //if a $ is anywhere inside this, let's do our thing on it
     if (wcschr(p_buf, '$') )
     {
       //there is a money symbol in it which means it may need var
       //replacing.  Let's do it.

       //make a buffer for it
         int i_orig_buff_size = wcslen(p_buf);
         int i_buff_size = (i_orig_buff_size *2)+C_MAX_URL_SIZE*2; //to be safe
         p_new_buff = new TCHAR[i_buff_size];

#ifdef _UNICODE
         ReplaceStrings(p_new_buff, p_buf, i_buff_size, i_orig_buff_size, NULL);
#else
         ReplaceStrings(p_new_buff, uni_buff.st_data, i_buff_size, i_orig_buff_size, NULL);

#endif
         p_buf_to_use = p_new_buff;
     }
  */

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

 //   SAFE_DELETE_ARRAY(p_buf);


}
