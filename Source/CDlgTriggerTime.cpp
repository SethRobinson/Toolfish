// CDlgTriggerTime.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgTriggerTime.h"
#include "CTrigger.h"
#include "..\Shared\uni.h"
#include "CDlgOkCancel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerTime dialog


CDlgTriggerTime::CDlgTriggerTime(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTriggerTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTriggerTime)
	m_b_reschedule_active = FALSE;
	m_cst_reschedule_time = _T("");
	m_b_ignore_late = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTriggerTime::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTriggerTime)
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_date);
	DDX_Control(pDX, IDC_TIME, m_time);
	DDX_Check(pDX, IDC_RESCHEDULE_ACTIVE, m_b_reschedule_active);
	DDX_Text(pDX, IDC_RESCHEDULE_TIME, m_cst_reschedule_time);
	DDX_Check(pDX, IDC_IGNORE_LATE, m_b_ignore_late);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTriggerTime, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTriggerTime)
	ON_BN_CLICKED(IDC_RESCHEDULE_ACTIVE, OnRescheduleActive)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerTime message handlers


BOOL CDlgTriggerTime::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
  SetBitmap(_T("gui\\background.bmp"));
	
    //copy info from the data
    CTime time_temp = m_p_trigger->GetTime();
    this->m_date.SetCurSel(time_temp);
    this->m_time.SetTime(&m_p_trigger->GetTime());

    this->m_b_reschedule_active = m_p_trigger->GetRescheduleActive();
    this->m_b_ignore_late = m_p_trigger->GetIgnoreLate();
    this->m_cst_reschedule_time.Format(_T("%d"), m_p_trigger->GetRescheduleRaw());

    CButton* p_button;

    if (m_p_trigger->GetRescheduleType() == C_TRIGGER_TIME_SECONDS)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_SECONDS);  
    }
    if (m_p_trigger->GetRescheduleType() == C_TRIGGER_TIME_MINUTES)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_MINUTES);  
    }
    if (m_p_trigger->GetRescheduleType() == C_TRIGGER_TIME_HOURS)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_HOURS);  
    }
    if (m_p_trigger->GetRescheduleType() == C_TRIGGER_TIME_DAYS)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_DAYS);  
    }

    p_button->SetCheck(true);
   
    UpdateData(D_TO_WINDOW);
    SetGreyOut();
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTriggerTime::SetGreyOut()
{

 ((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->EnableWindow(m_b_reschedule_active);
 ((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->EnableWindow(m_b_reschedule_active);
 ((CButton*)GetDlgItem(IDC_RADIO_HOURS))->EnableWindow(m_b_reschedule_active);
 ((CButton*)GetDlgItem(IDC_RADIO_DAYS))->EnableWindow(m_b_reschedule_active);

 ((CEdit*)GetDlgItem(IDC_RESCHEDULE_TIME))->EnableWindow(m_b_reschedule_active);


}

void CDlgTriggerTime::SetTriggerPointer(CTrigger *p_new)
{
    m_p_trigger = p_new;
}

void CDlgTriggerTime::OnOK() 
{
    //copy it all back in
    UpdateData(D_TO_VAR);

    
    long l_old_raw = m_p_trigger->GetRescheduleTimeInSeconds();

    if (m_b_reschedule_active)
    {
       if (_tcstol(m_cst_reschedule_time, NULL, 10) == 0)
       {
  this->MessageBox(_T("You have set the reschedule time to zero, this is invalid.\r\nTurn off the auto reschedule option or set it to a valid number."),
      _T("Error"));
         return;  
       }

    }
  
  if (((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->GetCheck()) 
      m_p_trigger->SetRescheduleType(C_TRIGGER_TIME_SECONDS);

    if (((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->GetCheck()) 
      m_p_trigger->SetRescheduleType(C_TRIGGER_TIME_MINUTES);

    if (((CButton*)GetDlgItem(IDC_RADIO_HOURS))->GetCheck()) 
      m_p_trigger->SetRescheduleType(C_TRIGGER_TIME_HOURS);
  
    if (((CButton*)GetDlgItem(IDC_RADIO_DAYS))->GetCheck()) 
      m_p_trigger->SetRescheduleType(C_TRIGGER_TIME_DAYS);

   
    m_p_trigger->SetRescheduleRaw(_tcstol(m_cst_reschedule_time, NULL, 10));
    

   CTime time2_temp;
   
   SYSTEMTIME sys_date;  
    m_date.GetCurSel(&sys_date);
     sys_date.wHour = sys_date.wMinute = sys_date.wSecond = sys_date.wMilliseconds =  0;

    CTime time_temp(sys_date);

    //log_msg("Time is %s.", (LPCSTR)time_temp.Format("%A, %B %d, %Y"));

    
    m_time.GetTime(time2_temp);
    
    
    CTime time_final(time_temp.GetYear(), time_temp.GetMonth(), time_temp.GetDay(),
       time2_temp.GetHour(), time2_temp.GetMinute(), time2_temp.GetSecond());
    
    m_p_trigger->SetTime(&time_final);
    m_p_trigger->SetRescheduleActive(m_b_reschedule_active != 0);
    m_p_trigger->SetIgnoreLate(m_b_ignore_late != 0);
    if (m_b_reschedule_active)
    if (m_p_trigger->GetRescheduleTimeInSeconds() < l_old_raw)
    {
       DlgOkCancel dlg;
       dlg.SetDialogText(_T("You changed the reschedule timing to be smaller, would you also like to set the next event time to run now? (instead of tomorrow or whatever it was set at)"));
       dlg.SetTitle(_T("Question"));
       if (dlg.DoModal() == IDOK)
       {
           m_p_trigger->SetTime(&CTime::GetCurrentTime());

       }


    }

    
    CBkDialogST::OnOK();
}

void CDlgTriggerTime::OnRescheduleActive() 
{
  UpdateData(D_TO_VAR);
  SetGreyOut();	
}

HBRUSH CDlgTriggerTime::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
