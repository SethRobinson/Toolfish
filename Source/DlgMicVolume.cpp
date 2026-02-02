// DlgMicVolume.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "DlgMicVolume.h"

#include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#undef THIS_FILE
#define THIS_FILE __FILE__

// DlgMicVolume dialog

IMPLEMENT_DYNAMIC(DlgMicVolume, CDialog)

DlgMicVolume::DlgMicVolume(CWnd* pParent /*=NULL*/)
	: CBkDialogST(DlgMicVolume::IDD, pParent)
	, m_partial_name(_T(""))
{

}

DlgMicVolume::~DlgMicVolume()
{
}

void DlgMicVolume::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Text(pDX, IDC_PARTIAL_NAME, m_partial_name);
}


BEGIN_MESSAGE_MAP(DlgMicVolume, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, &DlgMicVolume::OnNMCustomdrawSlider)
	ON_BN_CLICKED(IDCANCEL, &DlgMicVolume::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &DlgMicVolume::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgMicVolume message handlers

void DlgMicVolume::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


HBRUSH DlgMicVolume::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
          /*
          if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
              pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }
            */
/*     
		  if (
            (pWnd->GetDlgCtrlID() == IDC_PARTIAL_NAME)
     || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
      || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
                 )
        {
            // log_msg("Group box detected.");
            pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) m_hbrush;
            return hbr;
        }
		*/
        
        //default 
        
        //pDC->SetTextColor(MY_TEXTCOLOR);
        pDC->SetBkMode(TRANSPARENT);
    //    hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
         hbr=(HBRUSH) m_hbrush;

        return hbr;
        break;
      case CTLCOLOR_BTN:
       pDC->SetBkMode(TRANSPARENT);
              hbr=(HBRUSH) m_hbrush;
              return hbr;

        break;
 
    }

	return hbr;
}


void DlgMicVolume::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void DlgMicVolume::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(D_TO_VAR);    
	m_p_action->SetVolume(m_slider.GetPos());
	
#ifdef _UNICODE

	m_p_action->SetFilename((const TCHAR *)m_partial_name);

#else
	m_p_action->SetFilename(uni((LPCSTR)m_partial_name).us_data);

#endif
	
	OnOK();
}

BOOL DlgMicVolume::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	SetBitmap(_T("gui\\background.bmp"));

	m_partial_name = m_p_action->GetFilename();

	m_slider.SetRange(0, 100, TRUE);
	m_slider.SetPos(m_p_action->GetVolume());
	
	UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void DlgMicVolume::OnEnChangePartialName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CBkDialogST::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
