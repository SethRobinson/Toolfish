// CDlgActionVolume.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionVolume.h"
 #include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionVolume dialog


CDlgActionVolume::CDlgActionVolume(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionVolume::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionVolume)
	m_b_reset_smartmute = FALSE;
	m_b_fade = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgActionVolume::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionVolume)
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Check(pDX, IDC_UNMUTE, m_b_reset_smartmute);
	DDX_Check(pDX, IDC_FADE, m_b_fade);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionVolume, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionVolume)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionVolume message handlers

HBRUSH CDlgActionVolume::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
          ||            (pWnd->GetDlgCtrlID() == IDC_SLIDER)
 
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

BOOL CDlgActionVolume::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
	
  	m_slider.SetRange(0, 10, TRUE);
	m_slider.SetPos(m_p_action->GetVolume());
    m_b_reset_smartmute = m_p_action->GetTTS(); //reuse this again, sloppy? perhaps
  	m_b_fade = m_p_action->GetFade();
    UpdateData(D_TO_WINDOW);
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgActionVolume::OnOK() 
{
  UpdateData(D_TO_VAR);    
   m_p_action->SetTTS(m_b_reset_smartmute != 0);
   m_p_action->SetVolume(m_slider.GetPos());
   m_p_action->SetFade(m_b_fade != 0);
	
	CBkDialogST::OnOK();
}
