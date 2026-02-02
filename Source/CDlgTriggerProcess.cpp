// CDlgTriggerProcess.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgTriggerProcess.h"
#include "CTrigger.h"
#include "CDlgAlert.h"
#include "ProcessFindUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerProcess dialog


CDlgTriggerProcess::CDlgTriggerProcess(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTriggerProcess::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTriggerProcess)
	m_cst_search = _T("");
	m_b_focus = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTriggerProcess::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTriggerProcess)
	DDX_Control(pDX, IDC_VIEW, m_but_view);
	DDX_Control(pDX, IDC_FILENAME, m_combo_filename);
	DDX_Control(pDX, IDC_FOUND, m_combo_found);
	DDX_Text(pDX, IDC_SEARCH, m_cst_search);
	DDX_Check(pDX, IDC_FOCUS, m_b_focus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTriggerProcess, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTriggerProcess)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_VIEW, OnView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerProcess message handlers

HBRUSH CDlgTriggerProcess::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgTriggerProcess::OnInitDialog() 
{
    CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
    
    //let's setup the boxes
        m_but_view.DrawBorder(false, false);
  // m_but_view.SetURL(L"http://www.view.com")	;
   m_but_view.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(20,20,255), false);
   m_but_view.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0), false);
    m_but_view.DrawTransparent();


    m_combo_found.ResetContent();
    m_combo_found.AddString(_T("found"));
    m_combo_found.AddString(_T("not found"));
    
    m_combo_filename.ResetContent();
    m_combo_filename.AddString(_T("in a process .exe name (exact match)"));
    m_combo_filename.AddString(_T("in a process window title text (substring ok)"));
  
    
    m_combo_filename.SetCurSel(m_p_trigger->GetProcessSearchField());
    m_combo_found.SetCurSel(m_p_trigger->GetProcessFoundField());

    m_cst_search = m_p_trigger->GetBufferPointer();

    m_b_focus = m_p_trigger->GetProcessFocus(); 
    UpdateData(D_TO_WINDOW);
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTriggerProcess::OnOK() 
{
	UpdateData(D_TO_VAR);
   	
    m_p_trigger->SetProcessSearchField(m_combo_filename.GetCurSel());
    m_p_trigger->SetProcessFoundField(m_combo_found.GetCurSel());


    m_p_trigger->CopyToBuffer(uni(m_cst_search).us_data);
    m_p_trigger->SetProcessFocus(m_b_focus != 0);
	CBkDialogST::OnOK();
}

void CDlgTriggerProcess::OnView() 
{
	// TODO: Add your control notification handler code here
    CDlgAlert dlg;

    dlg.SetWindowCaption(_T("Active process information"));
    CString cst_text;

    BuildListOfActiveProcesses(cst_text);

    dlg.StoreTextPointerForLater(cst_text);

    dlg.DoModal();



}
