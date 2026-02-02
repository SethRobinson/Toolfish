// CDlgAction.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgAction.h"

#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAction dialog


CDlgAction::CDlgAction(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgAction::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAction)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAction::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAction)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAction, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgAction)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAction message handlers

void CDlgAction::SetActionToEdit(CAction *p_new)
{
    m_p_action = p_new;
}


void CDlgAction::OnOK() 
{
  m_p_action->SetActionType(m_list.GetCurSel());
 	
	CBkDialogST::OnOK();
}

void CDlgAction::OnDblclkList() 
{
  OnOK();
	
}

BOOL CDlgAction::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
 //fill the listbox up with possible action types to choose from
  SetBitmap(_T("gui\\background.bmp"));
    
    for (int i=0; i < g_actions.GetCount(); i++)
    {
        
        m_list.AddString(g_actions.m_action[i].st_name);
    }
    
     //set default
    m_list.SetCurSel(0);
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgAction::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
