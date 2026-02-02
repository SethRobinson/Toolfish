// CDlgMute.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgMute.h"
#include "CGlobals.h"      
#include "mute_util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMute dialog


CDlgMute::CDlgMute(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgMute::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMute)
	m_b_smart_mute = FALSE;
	m_cst_time = _T("");
	m_b_volume_knob_enabled = FALSE;
	m_cst_volume_knob_multiplier = _T("");
	m_b_volume_knob_shift_only = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgMute::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMute)
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Check(pDX, IDC_MUTE_ENABLED, m_b_smart_mute);
	DDX_Text(pDX, IDC_TIME, m_cst_time);
	DDX_Check(pDX, IDC_VOLUME_KNOB_ENABLED, m_b_volume_knob_enabled);
	DDX_Text(pDX, IDC_VOLUME_KNOB_MULTIPLIER, m_cst_volume_knob_multiplier);
	DDX_Check(pDX, IDC_VOLUME_KNOB_SHIFT_ONLY, m_b_volume_knob_shift_only);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMute, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgMute)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_MUTE_ENABLED, OnMuteEnabled)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_VOLUME_KNOB_ENABLED, OnVolumeKnobEnabled)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMute message handlers



BOOL CDlgMute::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	SetBitmap(_T("gui\\background.bmp"));

	m_slider.SetRange(0, 10, TRUE);
	m_slider.SetPos(glo.m_i_mute_volume);
    
    m_b_smart_mute = glo.m_b_smart_mute;
    m_cst_time.Format(_T("%d"), glo.m_i_mute_time);

    // Load volume knob settings
    m_b_volume_knob_enabled = glo.m_b_volume_knob_sensitivity;
    m_cst_volume_knob_multiplier.Format(_T("%d"), glo.m_i_volume_knob_multiplier);
    m_b_volume_knob_shift_only = glo.m_b_volume_knob_shift_only;

    //set radio buttons
    CButton* p_button;
    if (glo.m_i_mute_time_type == C_TIMER_TYPE_SECONDS)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_SECONDS);  
    }
    if (glo.m_i_mute_time_type ==C_TIMER_TYPE_MINUTES)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO_MINUTES);  
    }
 
    p_button->SetCheck(1);


    UpdateData(D_TO_WINDOW);
    SetGreyOut();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMute::OnCancel() 
{
	// TODO: Add your control notification handler code here
   CBkDialogST::OnCancel();
	
}


void CDlgMute::OnOk() 
{
    //set mute volume
    UpdateData(D_TO_VAR);
    glo.m_i_mute_volume =  m_slider.GetPos();
  	
    if ( ((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->GetCheck())
    {
        glo.m_i_mute_time_type = C_TIMER_TYPE_SECONDS;
    }

    if ( ((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->GetCheck())
    {
        glo.m_i_mute_time_type = C_TIMER_TYPE_MINUTES;
    }
    glo.m_i_mute_time = _tcstol(m_cst_time, NULL, 10);

    glo.m_b_smart_mute = m_b_smart_mute != 0;

    // Save volume knob settings
    glo.m_b_volume_knob_sensitivity = m_b_volume_knob_enabled != 0;
    int multiplier = _tcstol(m_cst_volume_knob_multiplier, NULL, 10);
    if (multiplier < 1) multiplier = 1;
    if (multiplier > 10) multiplier = 10;
    glo.m_i_volume_knob_multiplier = multiplier;
    glo.m_b_volume_knob_shift_only = m_b_volume_knob_shift_only != 0;

    FileConfigSave(&glo);
  
    app_glo.ResetMuteTimer();
    
    CBkDialogST::OnOK();
}

void ActivateMute()
{
    if (app_glo.GetMuted())
    {
        LogMsg(_T("Already muted!"));
        return;
    }

    ULONG mmin,mmax;
    app_glo.SetUnmuteVol(GetMasterVolume(mmin,mmax));

//	LogMsg("Cur vol is %d.",(int)app_glo.GetUnmuteVol());
          
          long lLeftVol = ( ((long)mmax - (long)mmin) / 10) * glo.m_i_mute_volume;
          if (mmin < 0)
          {
              //compensate for a negative based minimum value
              lLeftVol += mmin;
          }
          
     //     LogMsg("Setting vol to %d.",(int) lLeftVol);
          //now let's set the volume.
          SetMasterVolume((ULONG) lLeftVol);
          app_glo.SetMuted(true);

}

void DeactivateMute()
{
    if (app_glo.GetMuted())
    {
    
    SetMasterVolume((ULONG) app_glo.GetUnmuteVol());
    app_glo.SetMuted(false); //this should reset the mute timer too?
    }

}

void CDlgMute::SetGreyOut()
{

 ((CButton*)GetDlgItem(IDC_RADIO_SECONDS))->EnableWindow(m_b_smart_mute);
 ((CButton*)GetDlgItem(IDC_RADIO_MINUTES))->EnableWindow(m_b_smart_mute);
 ((CButton*)GetDlgItem(IDC_SLIDER))->EnableWindow(m_b_smart_mute);
 ((CEdit*)GetDlgItem(IDC_TIME))->EnableWindow(m_b_smart_mute);

 // Volume knob controls - enable sub-options only when main checkbox is checked
 ((CEdit*)GetDlgItem(IDC_VOLUME_KNOB_MULTIPLIER))->EnableWindow(m_b_volume_knob_enabled);
 ((CButton*)GetDlgItem(IDC_VOLUME_KNOB_SHIFT_ONLY))->EnableWindow(m_b_volume_knob_enabled);

}


void CDlgMute::OnMuteEnabled() 
{
	UpdateData(D_TO_VAR);
    SetGreyOut();
	
}

void CDlgMute::OnVolumeKnobEnabled() 
{
	UpdateData(D_TO_VAR);
    SetGreyOut();
	
}

HBRUSH CDlgMute::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
            || (pWnd->GetDlgCtrlID() == IDC_VOLUME_KNOB_GROUP)
            || (pWnd->GetDlgCtrlID() == IDC_SLIDER)
 
            )
        {
            // LogMsg("Group box detected.");
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
