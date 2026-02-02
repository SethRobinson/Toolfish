// CDlgActionSound.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgActionSound.h"
#include "CAction.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSound dialog


CDlgActionSound::CDlgActionSound(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionSound::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionSound)
	m_cst_filename = _T("");
	//}}AFX_DATA_INIT
}


void CDlgActionSound::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionSound)
	DDX_Text(pDX, IDC_FILENAME, m_cst_filename);
	DDV_MaxChars(pDX, m_cst_filename, 1900);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionSound, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionSound)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSound message handlers

void CDlgActionSound::OnBrowse() 
{
  	UpdateData(D_TO_VAR);
	//choose a file to be the graphic
    //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 _T(".wav"), //default file extension
		 _T(""), OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 _T("Sound file (*.*)|*.*|All files (*.*)|*.*|"),//file filter
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

void CDlgActionSound::OnOK() 
{
	UpdateData(D_TO_VAR);
	
#ifdef _UNICODE

    m_p_action->SetFilename((LPCWSTR)m_cst_filename);

#else
    m_p_action->SetFilename(uni((LPCSTR)m_cst_filename).us_data);

#endif

    CBkDialogST::OnOK();
}

BOOL CDlgActionSound::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	 SetBitmap(_T("gui\\background.bmp"));

    m_cst_filename = m_p_action->GetFilename();
 	UpdateData(D_TO_WINDOW);
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgActionSound::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
