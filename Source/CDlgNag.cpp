// CDlgNag.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgNag.h"
#include "..\Shared\all.h"
#include "..\\..\\shared\\std_all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"
#include "CDlgUnlock.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNag dialog


CDlgNag::CDlgNag(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgNag::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNag)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNag::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNag)
	DDX_Control(pDX, IDC_LOGO, m_but_logo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNag, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgNag)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PURCHASE, OnPurchase)
	ON_BN_CLICKED(IDC_ENTER_CODE, OnEnterCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNag message handlers

HBRUSH CDlgNag::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
 switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
        if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
          //    pDC->SetBkMode(TRANSPARENT);
            pDC->SetBkColor(RGB(255,255,255));
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }

        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
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
        hbr=(HBRUSH) (m_hbrush);
        return hbr;
        break;
      
    }
	

    
    return hbr;
}

BOOL CDlgNag::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	

     SetBitmap(_T("gui\\background.bmp"));
     
	 //setup graphic
     m_but_logo.DrawTransparent();
     m_but_logo.SetBitmaps(_T("gui\\but_wizard.bmp"), RGB(255,255,255), 0,0);
     //m_but_logo.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
     // m_but_logo.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
     m_but_logo.SizeToContent();
     m_but_logo.DrawBorder(false, false);
     
   

     //set the text to be correct

     if (app_glo.GetTrialDaysLeft() <= 0)
     {
         //sorry, no can do.
         ((CStatic*)GetDlgItem(IDC_NAG_TEXT))->SetWindowText(_T("Your trial period has expired."));
         
         //blank out continue button because they can't!!!
         (CButton*)GetDlgItem(IDOK)->EnableWindow(false);
         
     } else
     {
         CString cst_temp;
         cst_temp.Format(_T("You have %d days of your trial period remaining."), app_glo.GetTrialDaysLeft());
         ((CStatic*)GetDlgItem(IDC_NAG_TEXT))->SetWindowText(cst_temp);
     }
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNag::OnPurchase() 
{
    TCHAR * p_wst_filename = _T("www.rtsoft.com/toolfish/purchase.shtml");
    winall_create_url_file_full(uni(p_wst_filename).to_st());
  int result = (int)ShellExecute(NULL,_T("open"),_T("temp.url"), NULL,NULL, SW_MAXIMIZE);
  ProcessError(this->m_hWnd, result, _T("temp.url"),p_wst_filename); //show an error message if we couldn't open this

	
}

void CDlgNag::OnEnterCode() 
{
    //let the bitches try to register
    CDlgUnlock dlg;
    
    if (IDOK == dlg.DoModal())
    {
        ///succefull attempt.  continue to toolfish
        OnOK();
        return;
    }
    
}
