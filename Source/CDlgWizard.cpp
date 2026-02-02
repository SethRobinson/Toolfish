// CDlgWizard.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgWizard.h"
#include "..\Shared\uni.h"
#include "CAction.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CDlgTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWizard dialog


CDlgWizard::CDlgWizard(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWizard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWizard)
	m_b_pop_up = FALSE;
	m_b_say_it = FALSE;
	m_st_filename = _T("");
	m_st_content_percent = _T("");
	m_st_check_time = _T("");
	m_b_log_it = FALSE;
	m_b_play_sound = FALSE;
	m_cst_say = _T("");
	m_b_browser = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgWizard::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWizard)
	DDX_Control(pDX, IDC_PIC, m_but_pic);
	DDX_Check(pDX, IDC_POP_UP, m_b_pop_up);
	DDX_Check(pDX, IDC_SAY_IT, m_b_say_it);
	DDX_Text(pDX, IDC_FILENAME, m_st_filename);
	DDV_MaxChars(pDX, m_st_filename, 1800);
	DDX_Text(pDX, IDC_EDIT_CONTENT_PERCENT, m_st_content_percent);
	DDX_Text(pDX, IDC_EDIT_CHECK_TIME, m_st_check_time);
	DDX_Check(pDX, IDC_LOG_IT, m_b_log_it);
	DDX_Check(pDX, IDC_PLAY_SOUND, m_b_play_sound);
	DDX_Text(pDX, IDC_SAY, m_cst_say);
	DDX_Check(pDX, IDC_BROWSER, m_b_browser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWizard, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWizard)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDC_SAY_IT, OnSayIt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWizard message handlers

BOOL CDlgWizard::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
	
    m_but_pic.SetTooltipText(_T("It's easy and fast to add a web page compare event!"));
   m_but_pic.DrawTransparent();
   m_but_pic.SetBitmaps(_T("gui\\but_wizard.bmp"), RGB(255,255,255), 0,0);
   //m_but_pic.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
  // m_but_pic.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
   m_but_pic.SizeToContent();
   m_but_pic.DrawBorder(false, false);
 
   m_st_filename = L"http://www.rtsoft.com";

    m_st_content_percent.Format(_T("%d"), 5);

   m_st_check_time.Format(_T("%d"), 6);

    ((CButton*)GetDlgItem(IDC_RADIO_HOURS))->SetCheck(true);
    m_b_log_it= true;
    m_b_say_it = true;
    m_b_play_sound = false;
    m_b_pop_up = false;

    m_cst_say = _T("$LAST_COMPARE_URL$ was updated.");
  
    
    UpdateData(D_TO_WINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgWizard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgWizard::OnTest() 
{
    UpdateData(D_TO_VAR);
 
   CDlgTest dlg;
   
   
   _tcscpy(dlg.m_wst_url, m_st_filename);
  
   dlg.DoModal();

}

void CDlgWizard::OnOK() 
{
    UpdateData(D_TO_VAR);
    //actually create the event 
    
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
    
   if (((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->GetCheck()) 
     p_trig->SetRescheduleType(C_TRIGGER_TIME_SECONDS);

    if (((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->GetCheck()) 
      p_trig->SetRescheduleType(C_TRIGGER_TIME_MINUTES);

    if (((CButton*)GetDlgItem(IDC_RADIO_HOURS))->GetCheck()) 
     p_trig->SetRescheduleType(C_TRIGGER_TIME_HOURS);
  
    if (((CButton*)GetDlgItem(IDC_RADIO_DAYS))->GetCheck()) 
     p_trig->SetRescheduleType(C_TRIGGER_TIME_DAYS);

   
    p_trig->SetRescheduleRaw(_tcstol(m_st_check_time, NULL, 10));
   app_glo.GetEventList()->SaveEvent(i_event_id);
 
   //action add the webcompare itself
   int i_action_id;
   CAction *p_action = NULL;

     i_action_id = p_event->AddAction();
     p_action = p_event->GetAction(i_action_id);

     p_action->SetActionType(C_ACTION_COMPARE);
#ifdef _UNICODE
     p_action->SetFilename(m_st_filename);
#else
     p_action->SetFilename(uni(m_st_filename).us_data);

#endif
     p_action->m_b_content = true;
    p_action->m_f_content_percent = float(_tcstol(m_st_content_percent, NULL, 10))/100;
  
   
   //great, now add the extra events if they want them.
   
  //check to see if we should add a sound file to play


      if  ((m_b_play_sound) )
   {
     i_action_id = p_event->AddAction();
     p_action = p_event->GetAction(i_action_id);

      p_action->SetActionType(C_ACTION_SOUND);
      p_action->SetFilename(L"media\\secret.wav");
   }
 
   
  

   if  (m_b_say_it || (m_b_log_it))
   {
       i_action_id = p_event->AddAction();
       p_action = p_event->GetAction(i_action_id);
       p_action->SetActionType(C_ACTION_MESSAGE);
       p_action->CopyToBuffer(uni(m_cst_say).us_data);
       p_action->m_b_say = m_b_say_it != 0;
       p_action->m_b_log = m_b_log_it != 0;
    
   }

 if  (m_b_browser)
   {
       i_action_id = p_event->AddAction();
       p_action = p_event->GetAction(i_action_id);
       p_action->SetActionType(C_ACTION_WEBSITE);
       p_action->SetFilename(uni(m_st_filename).us_data);
   }

   if  ((m_b_pop_up))
   {
     i_action_id = p_event->AddAction();
     p_action = p_event->GetAction(i_action_id);

      p_action->SetActionType(C_ACTION_MESSAGE);
      
      if (m_cst_say.Compare(_T("$LAST_COMPARE_URL$ was updated.")) == 0)
      {
        //they didn't change the default text, so LAST_COMPARE_URL is already in it.
        p_action->CopyToBuffer(uni(m_cst_say).us_data);
  
      }   else
      {
          //let's pop up what they want, but also add the URL.
  
          CString cst_temp;
          cst_temp.Format(_T("%s\r\n($LAST_COMPARE_URL$)"), m_cst_say);
          p_action->CopyToBuffer(uni(cst_temp).us_data);
      }
   
      p_action->m_b_popup = m_b_pop_up != 0;
   }


  

      //let's save it now too to be safe
   app_glo.GetEventList()->SaveEvent(i_event_id);
   
    CBkDialogST::OnOK();
}

void CDlgWizard::OnSayIt() 
{
   UpdateData(D_TO_VAR);

   (CButton*)GetDlgItem(IDC_SAY)->EnableWindow(m_b_say_it);
	
}
