// CDlgTrigger.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgTrigger.h"
#include "CTrigger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTrigger dialog


CDlgTrigger::CDlgTrigger(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTrigger::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTrigger)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTrigger::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTrigger)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTrigger, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTrigger)
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTrigger message handlers

void CDlgTrigger::OnOK() 
{
    m_p_trigger->SetTriggerType(m_list.GetCurSel());
    
    CBkDialogST::OnOK();
}

void CDlgTrigger::OnCancel() 
{
	CBkDialogST::OnCancel();
}


void CDlgTrigger::SetTriggerToEdit(CTrigger *p_new)
{
    m_p_trigger = p_new;
}



BOOL CDlgTrigger::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
 SetBitmap(_T("gui\\background.bmp"));
  
	
    //fill the listbox up with possible trigger types to choose from
    
    for (int i=0; i < g_triggers.GetCount(); i++)
    {
        
        m_list.AddString(g_triggers.m_trigger[i].st_name);
    }
    
     //set default
    m_list.SetCurSel(0);



    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTrigger::OnDblclkList() 
{
  OnOK();
	
}

HBRUSH CDlgTrigger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
