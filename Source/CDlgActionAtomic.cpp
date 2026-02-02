// CDlgActionAtomic.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionAtomic.h"
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
// CDlgActionAtomic dialog


CDlgActionAtomic::CDlgActionAtomic(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionAtomic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionAtomic)
	m_cst_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgActionAtomic::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionAtomic)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_CBString(pDX, IDC_COMBO, m_cst_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionAtomic, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionAtomic)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionAtomic message handlers

HBRUSH CDlgActionAtomic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
       ||     (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
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

void InitAtomicComboBox(CComboBox &combo)
{
   
    combo.Clear();
    combo.AddString(_T("time-a.nist.gov"));
    combo.AddString(_T("time-b.nist.gov"));
 //   combo.AddString(_T("me-b.timefreq.bldrdoc.gov"));
    combo.AddString(_T("time-nw.nist.gov"));
    combo.AddString(_T("nist1.datum.com"));
    combo.AddString(_T("nist1-dc.glassey.com"));
    combo.AddString(_T("nist1.aol-ca.truetime.com"));
   
}

BOOL CDlgActionAtomic::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
 SetBitmap(_T("gui\\background.bmp"));

   InitAtomicComboBox(m_combo);


 m_cst_text = m_p_action->GetEmailServer();
 UpdateData(D_TO_WINDOW);
  if (m_cst_text.GetLength() == 0)
  {
      //set a default.
      m_combo.SelectString(0, _T("time-a.nist.gov"));
  }

  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionAtomic::OnOK() 
{
	//crap the crap back
    UpdateData(D_TO_VAR);
    m_p_action->SetEmailServer(uni(m_cst_text).us_data);


	CBkDialogST::OnOK();
}
