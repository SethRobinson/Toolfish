// CDlgFun.cpp : implementation file
//
#pragma once

#include "stdafx.h"
#include "source.h"
#include "CDlgFun.h"
#include "CDlgStatistics.h"
#include "CGlobals.h"  
#include "spy_util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFun dialog


CDlgFun::CDlgFun(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgFun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFun)
	m_b_overlay = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgFun::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFun)
	DDX_Check(pDX, IDC_OVERLAY, m_b_overlay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFun, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgFun)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STATISTICS, OnStatistics)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFun message handlers

HBRUSH CDlgFun::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
          ||    (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
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

BOOL CDlgFun::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
	SetBitmap(_T("gui\\background.bmp"));
	m_b_overlay = glo.m_b_key_overlay_active;
    UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFun::OnStatistics() 
{
	// TODO: Add your control notification handler code here
	CDlgStatistics dlg;
    dlg.DoModal();
}

void CDlgFun::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(D_TO_VAR);
    glo.m_b_key_overlay_active = m_b_overlay != 0;
    SetupKeyboardOverlay();

	CBkDialogST::OnOK();
}
                                                   
