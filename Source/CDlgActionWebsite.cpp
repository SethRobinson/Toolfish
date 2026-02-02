// CDlgActionWebsite.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgActionWebsite.h"
#include "CAction.h"
#include "..\Shared\uni.h"
#include "..\\..\\shared\\std_all.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionWebsite dialog


CDlgActionWebsite::CDlgActionWebsite(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionWebsite::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionWebsite)
	m_filename = _T("");
	m_b_new = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgActionWebsite::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionWebsite)
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	DDX_Check(pDX, IDC_NEW, m_b_new);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionWebsite, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionWebsite)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionWebsite message handlers

BOOL CDlgActionWebsite::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
  SetBitmap(_T("gui\\background.bmp"));
	
	m_filename = m_p_action->GetFilename();

    if (m_filename.GetLength() == 0)
    {
        m_filename = L"http://www.rtsoft.com";
    }
    m_b_new = !m_p_action->GetOpenInSameWindow();
    UpdateData(D_TO_WINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionWebsite::OnOK() 
{
	UpdateData(D_TO_VAR);
#ifdef _UNICODE
    m_p_action->SetFilename(m_filename)	;
#else
    m_p_action->SetFilename(uni(m_filename).us_data);

#endif
   m_p_action->SetOpenInSameWindow(m_b_new != 1); //yes I want to reverse this
    
    CBkDialogST::OnOK();
}

void CDlgActionWebsite::OnTest() 
{
  UpdateData(D_TO_VAR);
    winall_create_url_file_full(uni(m_filename).to_st());
  int result = (int)ShellExecute(NULL,_T("open"),_T("temp.url"), NULL,NULL, SW_MAXIMIZE);

#ifdef _UNICODE
  ProcessError(this->m_hWnd, result, _T("temp.url"), m_filename); //show an error message if we couldn't open this
#else

  char st_file[MAX_PATH];
  strcpy(st_file, m_filename);
  ProcessError(this->m_hWnd, result, _T("temp.url"), st_file); //show an error message if we couldn't open this
  
#endif
	
}

HBRUSH CDlgActionWebsite::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
