// CDlgEvent.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgEvent.h"
#include "CEvent.h"
#include "CGLobals.h"
#include "CDlgTrigger.h"
#include "CDlgAction.h"   
#include "..\Shared\uni.h"
#include "..\Shared\mfc_all.h"
#include "CDlgRename.h"
#include "std_all.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEvent dialog


CDlgEvent::CDlgEvent(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgEvent::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEvent)
	m_cst_name = _T("");
	m_b_enabled = FALSE;
	m_b_auto_name = FALSE;
	m_cst_filename = _T("");
	m_b_quiet = FALSE;
	m_b_auto_delete_event_after_running = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgEvent::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEvent)
	DDX_Control(pDX, IDC_ACTION_UP, m_but_action_up);
	DDX_Control(pDX, IDC_ACTION_DOWN, m_but_action_down);
	DDX_Control(pDX, IDC_TRIGGER_UP, m_but_trigger_up);
	DDX_Control(pDX, IDC_TRIGGER_DOWN, m_but_down);
	DDX_Control(pDX, IDC_ACTION_LIST, m_list_action);
	DDX_Control(pDX, IDC_TRIGGER_LIST, m_list_trigger);
	DDX_Text(pDX, IDC_NAME, m_cst_name);
	DDX_Check(pDX, IDC_ENABLED, m_b_enabled);
	DDX_Check(pDX, IDC_EVENT_NAME, m_b_auto_name);
	DDX_Text(pDX, IDC_FILENAME, m_cst_filename);
	DDX_Check(pDX, IDC_QUIET, m_b_quiet);
	DDX_Check(pDX, IDC_AUTO_DELETE_AFTER_RUNNING, m_b_auto_delete_event_after_running);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEvent, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgEvent)
	ON_EN_KILLFOCUS(IDC_NAME, OnKillfocusName)
	ON_BN_CLICKED(IDC_TRIGGER_ADD, OnTriggerAdd)
	ON_BN_CLICKED(IDC_TRIGGER_DELETE, OnTriggerDelete)
	ON_BN_CLICKED(IDC_TRIGGER_EDIT, OnTriggerEdit)
	ON_LBN_DBLCLK(IDC_TRIGGER_LIST, OnDblclkTriggerList)
	ON_BN_CLICKED(IDC_ENABLED, OnEnabled)
	ON_BN_CLICKED(IDC_ACTION_ADD, OnActionAdd)
	ON_BN_CLICKED(IDC_ACTION_DELETE, OnActionDelete)
	ON_BN_CLICKED(IDC_ACTION_EDIT, OnActionEdit)
	ON_LBN_DBLCLK(IDC_ACTION_LIST, OnDblclkActionList)
	ON_BN_CLICKED(IDC_EVENT_NAME, OnEventName)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TRIGGER_DOWN, OnTriggerDown)
	ON_BN_CLICKED(IDC_ACTION_UP, OnActionUp)
	ON_BN_CLICKED(IDC_ACTION_DOWN, OnActionDown)
	ON_BN_CLICKED(IDC_TRIGGER_UP, OnTriggerUp)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEvent message handlers

BOOL CDlgEvent::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	if (SetBitmap(_T("gui\\background.bmp")) != BKDLGST_OK)
    {
        log_error("Unable to load dialog background.");
    }
    //setup the buttons
   

    //trigger
    m_but_down.SetTooltipText(_T("Use these arrows to change the order they run in."));
    m_but_down.DrawTransparent();
    m_but_down.SetBitmaps(_T("gui\\arrow_down.bmp"), RGB(255,255,255), NULL, RGB(255,255,255));
    m_but_down.SizeToContent();
   
    m_but_trigger_up.SetTooltipText(_T("Use these arrows to change the order they run in."));
    m_but_trigger_up.DrawTransparent();
    m_but_trigger_up.SetBitmaps(_T("gui\\arrow_up.bmp"), RGB(255,255,255), NULL, RGB(255,255,255));
    m_but_trigger_up.SizeToContent();
   
    m_but_action_up.SetTooltipText(_T("Use these arrows to change the order they run in."));
    m_but_action_up.DrawTransparent();
    m_but_action_up.SetBitmaps(_T("gui\\arrow_up.bmp"), RGB(255,255,255), NULL, RGB(255,255,255));
    m_but_action_up.SizeToContent();
 
    m_but_action_down.SetTooltipText(_T("Use these arrows to change the order they run in."));
    m_but_action_down.DrawTransparent();
    m_but_action_down.SetBitmaps(_T("gui\\arrow_down.bmp"), RGB(255,255,255), NULL, RGB(255,255,255));
    m_but_action_down.SizeToContent();

	//copy stuff into the edit boxes
    m_cst_name = m_p_event->GetName();
    m_b_enabled = m_p_event->GetEnabled();
    m_b_auto_name = m_p_event->GetAutoNameEnabled();
    m_b_quiet = m_p_event->GetQuietMode();
    m_b_auto_delete_event_after_running = m_p_event->GetAutoDeleteAfterRunning();
    
    if (m_b_auto_name) m_cst_name = m_p_event->GetAutoName(false);
    
    RebuildTriggerList();
    RebuildActionList();

    //but when was it last run?

	CTime timeLastRun = m_p_event->GetTimeLastRunAsCTime();

    if (timeLastRun == 0)
    {
        ((CStatic*)GetDlgItem(IDC_LAST_RUN))->SetWindowText(_T("Never"));
    } else
    {
        CString st_temp;
        CString st_since;
        CString cst_am = "AM";

        int i_hour = timeLastRun.GetHour();
        if (i_hour > 12)
        {
            cst_am = "PM";
            i_hour -= 12;
        }

        if (i_hour == 0)
        {
            i_hour = 12; //adjust for 
        }

        int i_minutes = timeLastRun.GetMinute();
        
		get_time_difference_in_text_long(&st_since, static_cast<time_t>(m_p_event->GetTimeLastRunAsCTime().GetTime()));
        
        
        st_temp.Format(_T("%s at %2.d:%2.d %s (%s ago)"), 
             timeLastRun.Format(_T("%A, %B %d, %Y")), 
             i_hour, i_minutes, cst_am,  st_since);
        
        ((CStatic*)GetDlgItem(IDC_LAST_RUN))->SetWindowText(st_temp);
    }

#ifdef _UNICODE 
    m_cst_filename.Format(_T("Event saved as \"%s\""), m_p_event->GetFileName());
#else
    m_cst_filename.Format(_T("Event saved as \"%s\""), uni(m_p_event->GetFileName()).to_st());

#endif
    
    UpdateData(D_TO_WINDOW);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

    void CDlgEvent::CalculateNextRun()
    {
      
       CString cst_text = _T("N/A (only applicable to time triggers)");

        //if it's a lone time triggger, let's tell them when it will run

        if (m_p_event->GetTriggerCount() == 1
            && m_p_event->GetTrigger(0)->GetTriggerType() == C_TRIGGER_TIME)
        {

          
            time_t t_now;
            time(&t_now);
            long i_seconds = static_cast<long>(difftime(static_cast<time_t>(m_p_event->GetTrigger(0)->GetTime().GetTime()), t_now));
 
            if (i_seconds > 0)
            {
            
            TCHAR *p_time = ShowTimePeriod(i_seconds);
            cst_text.Format(_T("In %s"), p_time);
            SAFE_DELETE_ARRAY(p_time);
            } else
            {
                //it's not in the future!
                cst_text = _T("Warning, timer is currently scheduled for a date in the past");
              }

        }
            
        if (!m_b_enabled) 
        {
            cst_text = _T("Never, it's disabled");
            

        }
        
        ((CStatic*)GetDlgItem(IDC_NEXT_RUN))->SetWindowText(cst_text);
  
        
    }
    
void CDlgEvent::SetEventToEdit(CEvent *p_new)
{
    m_p_event = p_new;
}

void CDlgEvent::OnKillfocusName() 
{
    UpdateData(D_TO_VAR);

#ifdef _UNICODE
    m_p_event->SetName(m_cst_name);

#else
    m_p_event->SetName(uni(m_cst_name).us_data);

#endif
}

void CDlgEvent::OnTriggerAdd() 
{
       UpdateData(D_TO_VAR);

    //add a new trigger
    int i_index = m_p_event->AddTrigger();
    //edit it
      CDlgTrigger dlg;
        dlg.SetTriggerToEdit( m_p_event->GetTrigger(i_index));
        dlg.DoModal();
     
        //if the trigger type is -1 then they hit cancel and we don't want it
        if (m_p_event->GetTrigger(i_index)->GetTriggerType() == -1)
        {
            m_p_event->DeleteTrigger(i_index);
        }  else
        {
            //edit it now
        CDialog *p_dlg = NULL;
        p_dlg = m_p_event->GetTrigger(i_index)->InitTriggerDialog();
        
        if (p_dlg)
        {
            
            if (p_dlg->DoModal() == IDCANCEL)
            {
                //canceled it
                  m_p_event->DeleteTrigger(i_index);
                  i_index = -1;
            }
            delete p_dlg;
        }
        }

     //either way, rebuild the list
    //rebuild trigger view pane
    RebuildTriggerList();
    if (i_index > -1)
     set_selection_by_data(&m_list_trigger, i_index);


}

void CDlgEvent::RebuildTriggerList()
{
    m_list_trigger.ResetContent();

    for (int i=0; i < m_p_event->GetTriggerCount(); i++)
    {
        if (m_p_event->GetTrigger(i))
        {
            //valid trigger.  deal with it
           int i_index = m_list_trigger.AddString(m_p_event->GetTrigger(i)->GetName());
           m_list_trigger.SetItemData(i_index, i);
        }

    }
   SetName();
   CalculateNextRun();
}



void CDlgEvent::OnTriggerDelete() 
{
   //get selection and delete it
    if (m_list_trigger.GetCurSel() > -1)
    {
        m_p_event->DeleteTrigger(m_list_trigger.GetItemData(m_list_trigger.GetCurSel()));
        RebuildTriggerList();
    }   else
    {
        log_msg("Nothing highlighted.");
    }
	
}

void CDlgEvent::OnTriggerEdit() 
{
       UpdateData(D_TO_VAR);

    if (m_list_trigger.GetCurSel() > -1)
    {
        int i_index = m_list_trigger.GetItemData(m_list_trigger.GetCurSel());
        CDialog *p_dlg = NULL;
        p_dlg = m_p_event->GetTrigger(i_index)->InitTriggerDialog();
       // dlg.SetTriggerToEdit( m_p_event->GetTrigger();
        if (p_dlg)
        {
        
        p_dlg->DoModal();
        delete p_dlg;
        RebuildTriggerList();
        set_selection_by_data(&m_list_trigger, i_index);


        }
    }   
        
        else
    {
        log_msg("Nothing highlighted.");
    }

	
}

void CDlgEvent::OnDblclkTriggerList() 
{
   OnTriggerEdit();	
}

void CDlgEvent::OnEnabled() 
{
	// TODO: Add your control notification handler code here
    UpdateData(D_TO_VAR);
    RebuildTriggerList();
}

void CDlgEvent::OnOK() 
{
	// TODO: Add extra validation here
    
     UpdateData(D_TO_VAR);
	 m_p_event->SetEnabled(m_b_enabled != 0);
     m_p_event->SetAutoNameEnabled(m_b_auto_name != 0);
     m_p_event->SetQuietMode(m_b_quiet != 0);
     m_p_event->SetAutoDeleteAfterRunning(m_b_auto_delete_event_after_running != 0);
     m_p_event->Save();
     CBkDialogST::OnOK();
}

void CDlgEvent::RebuildActionList()
{
    m_list_action.ResetContent();

    for (int i=0; i < m_p_event->GetActionCount(); i++)
    {
        if (m_p_event->GetAction(i))
        {
            //valid action.  deal with it
           int i_index = m_list_action.AddString(m_p_event->GetAction(i)->GetName(false));
           m_list_action.SetItemData(i_index, i);
        }

    }
     SetName();

}



void CDlgEvent::OnActionAdd() 
{
       UpdateData(D_TO_VAR);

    //add a new action
    int i_index = m_p_event->AddAction();
    //edit it
      CDlgAction dlg;
        dlg.SetActionToEdit( m_p_event->GetAction(i_index));
        dlg.DoModal();
     
        //if the action type is -1 then they hit cancel and we don't want it
        if (m_p_event->GetAction(i_index)->GetActionType() == -1)
        {
            m_p_event->DeleteAction(i_index);
        }  else
        {
            //edit it now
      CDialog *p_dlg = NULL;
       p_dlg = m_p_event->GetAction(i_index)->InitActionDialog();
        if (p_dlg->DoModal() == IDCANCEL)
        {
            //log_msg("It was canceled!!!");
             m_p_event->DeleteAction(i_index);
             i_index = -1;
        }
        delete p_dlg;
        
        

        }

     //either way, rebuild the list
    //rebuild action view pane
    RebuildActionList();
     if (i_index > -1)
     set_selection_by_data(&m_list_action, i_index);

  

}

void CDlgEvent::OnActionDelete() 
{
 //get selection and delete it
    if (m_list_action.GetCurSel() > -1)
    {
        m_p_event->DeleteAction(m_list_action.GetItemData(m_list_action.GetCurSel()));
        RebuildActionList();
    }   else
    {
        log_msg("Nothing highlighted.");
    }

}

void CDlgEvent::OnActionEdit() 
{

       UpdateData(D_TO_VAR);

    if (m_list_action.GetCurSel() > -1)
    {
        int i_index = m_list_action.GetItemData(m_list_action.GetCurSel());
        CDialog *p_dlg = NULL;
        p_dlg = m_p_event->GetAction(i_index)->InitActionDialog();
       // dlg.SetActionToEdit( m_p_event->GetAction();
        if (p_dlg)
        {
        
        p_dlg->DoModal();
        delete p_dlg;
        RebuildActionList();
        set_selection_by_data(&m_list_action, i_index);


        }
    }   
        
        else
    {
        log_msg("Nothing highlighted.");
    }


}

void CDlgEvent::OnDblclkActionList() 
{
  OnActionEdit();	
	
}

void CDlgEvent::SetName()
{
  

      if (m_b_auto_name) m_cst_name = m_p_event->GetAutoName(false);
      ((CWnd*)GetDlgItem(IDC_NAME))->EnableWindow(!m_b_auto_name);

   UpdateData(D_TO_WINDOW);



      //grey or ungrey it out, depending
}

void CDlgEvent::OnEventName() 
{
    UpdateData(D_TO_VAR);
 
    SetName();
   	//if (m_b_auto_name)
    //Invalidate();
}

HBRUSH CDlgEvent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (nCtlColor)
	{
	
	// TODO: Change any attributes of the DC here
	 case CTLCOLOR_STATIC:  
     
  // 	case CTLCOLOR_EDIT:
//	case CTLCOLOR_SCROLLBAR:
//	case CTLCOLOR_LISTBOX:
       
         if (
             (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
             ||
             (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
             ||
             (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
             )
         {
            // log_msg("Group box detected.");
              pDC->SetBkMode(TRANSPARENT);
              hbr=(HBRUSH) m_hbrush;
              return hbr;
        }

         //pDC->SetTextColor(MY_TEXTCOLOR);
        pDC->SetBkMode(TRANSPARENT);
        // hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
         hbr=(HBRUSH) m_hbrush;
         return hbr;
         break;
     case CTLCOLOR_BTN:
       pDC->SetBkMode(TRANSPARENT);
              hbr=(HBRUSH) m_hbrush;
              return hbr;

        break;

    }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgEvent::OnTriggerDown() 
{
	  if (m_list_trigger.GetCurSel() == -1)
      {
          log_msg("Can't move trigger, nothing selected.");
          return;
      }

    
    int i_trig = m_list_trigger.GetItemData(m_list_trigger.GetCurSel());

    //let's move an event down in the list, first check to see if that's a doable proposition or not
    int i_new = m_p_event->MoveTriggerDown(i_trig);

    //rebuild the list, but keep the right one selected
    RebuildTriggerList();
    set_selection_by_data(&m_list_trigger, i_new);
}

void CDlgEvent::OnActionUp() 
{
if (m_list_action.GetCurSel() == -1)
      {
          log_msg("Can't move action, nothing selected.");
          return;
      }

    
    int i_trig = m_list_action.GetItemData(m_list_action.GetCurSel());

    //let's move an event down in the list, first check to see if that's a doable proposition or not
    int i_new = m_p_event->MoveActionUp(i_trig);

    //rebuild the list, but keep the right one selected
    RebuildActionList();
    set_selection_by_data(&m_list_action, i_new);
	
}

void CDlgEvent::OnActionDown() 
{
  if (m_list_action.GetCurSel() == -1)
      {
          log_msg("Can't move action, nothing selected.");
          return;
      }

    
    int i_trig = m_list_action.GetItemData(m_list_action.GetCurSel());

    //let's move an event down in the list, first check to see if that's a doable proposition or not
    int i_new = m_p_event->MoveActionDown(i_trig);

    //rebuild the list, but keep the right one selected
    RebuildActionList();
    set_selection_by_data(&m_list_action, i_new);
	
}

void CDlgEvent::OnTriggerUp() 
{
  if (m_list_trigger.GetCurSel() == -1)
      {
          log_msg("Can't move trigger, nothing selected.");
          return;
      }

    
    int i_trig = m_list_trigger.GetItemData(m_list_trigger.GetCurSel());

    //let's move an event down in the list, first check to see if that's a doable proposition or not
    int i_new = m_p_event->MoveTriggerUp(i_trig);

    //rebuild the list, but keep the right one selected
    RebuildTriggerList();
    set_selection_by_data(&m_list_trigger, i_new);
	
}

void CDlgEvent::OnRename() 
{
	UpdateData(D_TO_VAR);
    CDlgRename dlg;
    dlg.m_cst_filename = m_p_event->GetFileName();
    dlg.m_cst_filename.Replace(_T("events\\"), _T(""));
    
    if (dlg.DoModal() == IDOK)
    {
        if  (dlg.m_cst_filename.GetLength() > 0) 
          
        {
            CString cst_final;
            cst_final.Format(_T("events\\%s"), dlg.m_cst_filename);
#ifdef _UNICODE
            if (cst_final.Compare(m_p_event->GetFileName()) == 0) //ignore no change
#else
            if (cst_final.Compare(uni(m_p_event->GetFileName()).to_st()) == 0) //ignore no change
                       
#endif
            {
                return;
            }
     
            
            if (exist(cst_final))
            {
                MessageBox(_T("Error, that filename already exists.  Try another."));
                return;
            }
            
            //delete the old file
#ifdef _UNICODE

            _tunlink(m_p_event->GetFileName());

#else
       _tunlink(uni(m_p_event->GetFileName()).to_st());

#endif
            //let's rename the file.
#ifdef _UNICODE
            m_p_event->SetFileName((const TCHAR*)cst_final);
#else
            m_p_event->SetFileName(uni((const CHAR*)cst_final).us_data);

#endif
            
            //save it out, don't want to lose anything if we crash
            m_p_event->Save();
#ifdef _UNICODE
            m_cst_filename.Format(_T("Event saved as \"%s\""), m_p_event->GetFileName());
#else
            m_cst_filename.Format(_T("Event saved as \"%s\""), uni(m_p_event->GetFileName()).to_st());
            
#endif
            
            UpdateData(D_TO_WINDOW);
        //  RECT rect; 
        //  GetDlgItem(IDC_FILENAME)->GetUpdateRect(&rect);

         // InvalidateRect(&rect);
          

        }   else
        {
            //error, don't change it
        }


    }

}
