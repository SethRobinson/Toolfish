// CDlgActionRun.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionRun.h"
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
// CDlgActionRun dialog


CDlgActionRun::CDlgActionRun(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionRun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionRun)
	m_cst_filename = _T("");
	m_edit_parms = _T("");
	m_b_wait = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgActionRun::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionRun)
	DDX_Text(pDX, IDC_FILENAME, m_cst_filename);
	DDV_MaxChars(pDX, m_cst_filename, 1900);
	DDX_Text(pDX, IDC_EDIT_PARMS, m_edit_parms);
	DDX_Check(pDX, IDC_CHECK_WAIT, m_b_wait);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionRun, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionRun)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionRun message handlers

HBRUSH CDlgActionRun::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgActionRun::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
 SetBitmap(_T("gui\\background.bmp"));

   m_cst_filename = m_p_action->GetFilename();
 
   m_edit_parms = m_p_action->GetBufferPointer();  //might be null, wonder what happens then.
   m_b_wait = !m_p_action->m_b_no_wait;

   UpdateData(D_TO_WINDOW);
 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionRun::OnBrowse() 
{
	UpdateData(D_TO_VAR);
	//choose a file to be the graphic
    //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 _T(".*"), //default file extension
		 _T(""), OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 _T("File (*.*)|*.*|All files (*.*)|*.*|"),//file filter
		 AfxGetApp()->m_pMainWnd);
		TCHAR st_temp[MAX_PATH];
	 getdir(st_temp);

	 cfd.m_ofn.lpstrInitialDir = st_temp;
		 
	 if(cfd.DoModal() == IDOK)
		 {
	       m_cst_filename =  cfd.GetPathName();

			UpdateData(D_TO_WINDOW);
		 } else
		 {
		   LogMsg(_T("Did not load."));
		 }
	
}

void CDlgActionRun::OnOK() 
{
	UpdateData(D_TO_VAR);
    
#ifdef _UNICODE

    m_p_action->SetFilename((const TCHAR *)m_cst_filename);
    m_p_action->CopyToBuffer(m_edit_parms);

#else
   m_p_action->SetFilename(uni((LPCSTR)m_cst_filename).us_data);
    m_p_action->CopyToBuffer(uni(m_edit_parms).us_data);
  
#endif
    
    m_p_action->m_b_no_wait = !(m_b_wait != 0);
    CBkDialogST::OnOK();
}
