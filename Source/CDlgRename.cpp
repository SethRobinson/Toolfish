// CDlgRename.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgRename.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog


CDlgRename::CDlgRename(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRename)
	m_cst_filename = _T("");
	//}}AFX_DATA_INIT
}


void CDlgRename::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRename)
	DDX_Text(pDX, IDC_FILENAME, m_cst_filename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRename, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgRename)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRename message handlers

void CDlgRename::OnOK() 
{
	// TODO: Add extra validation here
	
	CBkDialogST::OnOK();
}

HBRUSH CDlgRename::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgRename::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
    SetBitmap(_T("gui\\background.bmp"));
   	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
