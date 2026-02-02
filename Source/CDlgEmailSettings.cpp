// CDlgEmailSettings.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"

#include "CAction.h"
#include "..\Shared\uni.h"
#include "CDlgOkCancel.h"
#include "CSMPTSetup.h"
#include "CDlgEmailSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgEmailSettings dialog


DlgEmailSettings::DlgEmailSettings(CWnd* pParent /*=NULL*/)
	: CBkDialogST(DlgEmailSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgEmailSettings)
	m_cst_subject = _T("");
	m_cst_from = _T("");
	m_cst_to = _T("");
	m_cst_mail_file = _T("");
	//}}AFX_DATA_INIT
}


void DlgEmailSettings::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgEmailSettings)
	DDX_Text(pDX, IDC_MAIL_SUBJECT, m_cst_subject);
	DDV_MaxChars(pDX, m_cst_subject, 255);
	DDX_Text(pDX, IDC_MAIL_FROM, m_cst_from);
	DDV_MaxChars(pDX, m_cst_from, 255);
	DDX_Text(pDX, IDC_MAIL_TO, m_cst_to);
	DDV_MaxChars(pDX, m_cst_to, 255);
	DDX_Text(pDX, IDC_MAIL_FILE, m_cst_mail_file);
	DDV_MaxChars(pDX, m_cst_mail_file, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgEmailSettings, CBkDialogST)
	//{{AFX_MSG_MAP(DlgEmailSettings)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_EDIT_SMTP, OnEditSmtp)
	ON_BN_CLICKED(IDC_FIND_FILE, OnFindFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgEmailSettings message handlers

HBRUSH DlgEmailSettings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
            ||            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
 
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

BOOL DlgEmailSettings::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	SetBitmap(_T("gui\\background.bmp"));

#ifdef _UNICODE
    m_cst_to = m_p_action->GetToEmail();
    m_cst_from = m_p_action->GetReturnEmail();
    m_cst_subject = m_p_action->GetEmailSubject();
    m_cst_mail_file = m_p_action->GetEmailFile();
#else
    m_cst_to = uni(m_p_action->GetToEmail()).to_st();
    m_cst_from = uni(m_p_action->GetReturnEmail()).to_st();
    m_cst_subject = uni(m_p_action->GetEmailSubject()).to_st();
    m_cst_mail_file =  uni(m_p_action->GetEmailFile()).to_st();; 
#endif

    //if values are null let's replace them with defaults
    if (m_cst_from.GetLength() == 0)
    {
#ifdef _UNICODE
        m_cst_from = glo.m_st_email;
#else
    m_cst_from = uni(glo.m_st_email).to_st();
#endif
    }
    if (m_cst_subject.GetLength() == 0) m_cst_subject = _T("Toolfish notification!");
    UpdateData(D_TO_WINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgEmailSettings::OnOK() 
{
   UpdateData(D_TO_VAR);
    //copy the stuff back out
#ifdef _UNICODE
    m_p_action->SetToEmail(m_cst_to);
    m_p_action->SetReturnEmail(m_cst_from);
    m_p_action->SetEmailSubject(m_cst_subject);
    m_p_action->SetEmailFile(m_cst_mail_file);
#else
    m_p_action->SetToEmail(uni(m_cst_to).us_data);
    m_p_action->SetReturnEmail(uni(m_cst_from).us_data);
    m_p_action->SetEmailSubject(uni(m_cst_subject).us_data);
    m_p_action->SetEmailFile(uni(m_cst_mail_file).us_data);

#endif

	CBkDialogST::OnOK();
}

void DlgEmailSettings::OnEditSmtp() 
{
    CSMPTSetup dlg;
    dlg.DoModal();

}

void DlgEmailSettings::OnFindFile() 
{
	UpdateData(D_TO_VAR);
  //load a new map	
 	 CFileDialog cfd(true, //true if open, false if close
		 _T(""), //default file extension
		 _T(""), OFN_HIDEREADONLY|OFN_NOCHANGEDIR ,
		 _T("All files (*.*)|*.*|"),//file filter
		 AfxGetApp()->m_pMainWnd);
		TCHAR st_temp[MAX_PATH];
	 getdir(st_temp);

	 cfd.m_ofn.lpstrInitialDir = st_temp;
		 
	 if(cfd.DoModal() == IDOK)
		 {
	       m_cst_mail_file =  cfd.GetPathName();

			UpdateData(D_TO_WINDOW);
		 } else
		 {
		 //  log_msg("Did not load.");
		 }

	
}
