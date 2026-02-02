// CDlgActionCompare.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgActionCompare.h"
#include "CAction.h"
#include "..\Shared\uni.h"
#include "CDlgOkCancel.h"
#include "CDlgTest.h"
#include "spy_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionCompare dialog


CDlgActionCompare::CDlgActionCompare(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionCompare::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionCompare)
	m_filename = _T("");
	m_b_content = FALSE;
	m_b_date = FALSE;
	m_b_words_found = FALSE;
	m_b_words_not_found = FALSE;
	m_st_content_percent = _T("");
	m_st_words_found = _T("");
	m_st_words_not_found = _T("");
	m_authentication = FALSE;
	m_st_auth_logon = _T("");
	m_st_auth_password = _T("");
	//}}AFX_DATA_INIT
}


void CDlgActionCompare::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionCompare)
	DDX_Text(pDX, IDC_FILENAME, m_filename);
	DDV_MaxChars(pDX, m_filename, 1900);
	DDX_Check(pDX, IDC_CHECK_CONTENT, m_b_content);
	DDX_Check(pDX, IDC_CHECK_DATE, m_b_date);
	DDX_Check(pDX, IDC_CHECK_WORDS_FOUND, m_b_words_found);
	DDX_Check(pDX, IDC_CHECK_WORDS_NOT_FOUND, m_b_words_not_found);
	DDX_Text(pDX, IDC_EDIT_CONTEST_PERCENT, m_st_content_percent);
	DDX_Text(pDX, IDC_ST_WORDS_FOUND, m_st_words_found);
	DDV_MaxChars(pDX, m_st_words_found, 127);
	DDX_Text(pDX, IDC_ST_WORDS_NOT_FOUND, m_st_words_not_found);
	DDV_MaxChars(pDX, m_st_words_not_found, 127);
	DDX_Check(pDX, IDC_AUTHENTICATION, m_authentication);
	DDX_Text(pDX, IDC_ST_AUTH_LOGON, m_st_auth_logon);
	DDX_Text(pDX, IDC_ST_AUTH_PASSWORD, m_st_auth_password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionCompare, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionCompare)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_AUTHENTICATION, OnAuthentication)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionCompare message handlers

BOOL CDlgActionCompare::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
   SetBitmap(_T("gui\\background.bmp"));
	m_filename = m_p_action->GetFilename();

    if (m_filename.GetLength() == 0)
    {
        m_filename = L"www.rtsoft.com";
    }

    m_b_content = m_p_action->m_b_content;
    m_b_date = m_p_action->m_b_date;
    m_b_words_found = m_p_action->m_b_words_found;
    m_b_words_not_found = m_p_action->m_b_words_not_found;

    //get percent change required
    m_st_content_percent.Format(_T("%d"), int(m_p_action->m_f_content_percent*100.01));

    m_st_words_found = m_p_action->m_ws_words_found;
    m_st_words_not_found = m_p_action->m_ws_words_not_found;

    m_authentication = m_p_action->GetWebCompareAuthenticate();
    m_st_auth_logon = m_p_action->GetLogon();
    m_st_auth_password = m_p_action->GetPassword();

    UpdateData(D_TO_WINDOW);
    ProcessGray();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgActionCompare::ProcessGray()
{
   
    ((CWnd*)GetDlgItem(IDC_ST_AUTH_LOGON))->EnableWindow(m_authentication);
    ((CWnd*)GetDlgItem(IDC_ST_AUTH_PASSWORD))->EnableWindow(m_authentication);
 
}


void CDlgActionCompare::OnTest() 
{
   UpdateData(D_TO_VAR);
 
   CDlgTest dlg;
   ReplaceStrings(dlg.m_wst_url, m_filename, C_MAX_URL_SIZE, m_filename.GetLength(), NULL);
     dlg.SetAuthentication(m_authentication, m_st_auth_logon, m_st_auth_password);
   dlg.DoModal();


}

void CDlgActionCompare::OnOK() 
{
		UpdateData(D_TO_VAR);
   
         if (!m_b_content) if (!m_b_date) if (!m_b_words_found) if (!m_b_words_not_found)
    {
             DlgOkCancel dlg;
             dlg.SetTitle(_T("Warning"));
             dlg.SetDialogText(_T("You have not enabled any comparing methods, this will always flag the trigger.  Is this what you want?"));
             
             int i_res = dlg.DoModal();

    if (i_res != IDOK)  return;
  }

        
        
        m_p_action->SetFilename(uni(m_filename).us_data);
            m_p_action->m_b_content = m_b_content != 0;
    m_p_action->m_b_date = m_b_date != 0;
    m_p_action->m_b_words_found = m_b_words_found != 0;
    m_p_action->m_b_words_not_found = m_b_words_not_found != 0;
    m_p_action->SetWebCompareAuthenticate(m_authentication != 0);
    m_p_action->SetLogon(uni(m_st_auth_logon).us_data);
    m_p_action->SetPassword(uni(m_st_auth_password).us_data);

    if (_tcstol(m_st_content_percent, NULL, 10) > 0)
    {
    
    m_p_action->m_f_content_percent = float(_tcstol(m_st_content_percent, NULL, 10))/100;
    } else
    {
        m_p_action->m_b_content = 0;
    }


    wcscpy(m_p_action->m_ws_words_found, uni(m_st_words_found).us_data);
    wcscpy(m_p_action->m_ws_words_not_found, uni(m_st_words_not_found).us_data);


	CBkDialogST::OnOK();
}

HBRUSH CDlgActionCompare::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
            ||            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
            

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

void CDlgActionCompare::OnAuthentication() 
{
    UpdateData(D_TO_VAR);
    ProcessGray();
    if (!m_authentication)
    {
        InvalidateRect(NULL);
    }

}
