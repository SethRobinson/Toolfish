// CDlgActionEmail.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionEmail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionEmail dialog

#include "CAction.h"

CDlgActionEmail::CDlgActionEmail(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionEmail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionEmail)
	m_cst_password = _T("");
	m_b_say_count = FALSE;
	m_b_say_from = FALSE;
	m_b_say_subject = FALSE;
	m_cst_logon = _T("");
	m_cst_server = _T("");
	m_b_filter_active = FALSE;
	m_i_filter_field = -1;
	m_i_filter_have = -1;
	m_cst_filter_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgActionEmail::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionEmail)
	DDX_Text(pDX, IDC_PASSWORD, m_cst_password);
	DDV_MaxChars(pDX, m_cst_password, 49);
	DDX_Check(pDX, IDC_SAY_COUNT, m_b_say_count);
	DDX_Check(pDX, IDC_SAY_FROM, m_b_say_from);
	DDX_Check(pDX, IDC_SAY_SUBJECT, m_b_say_subject);
	DDX_Text(pDX, IDC_NAME, m_cst_logon);
	DDV_MaxChars(pDX, m_cst_logon, 49);
	DDX_Text(pDX, IDC_SERVER, m_cst_server);
	DDV_MaxChars(pDX, m_cst_server, 2000);
	DDX_Check(pDX, IDC_FILTER_ACTIVE, m_b_filter_active);
	DDX_CBIndex(pDX, IDC_FILTER_FIELD, m_i_filter_field);
	DDX_CBIndex(pDX, IDC_FILTER_HAVE, m_i_filter_have);
	DDX_Text(pDX, IDC_FILTER_TEXT, m_cst_filter_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionEmail, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionEmail)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_FILTER_ACTIVE, OnFilterActive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionEmail message handlers

HBRUSH CDlgActionEmail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
          /*
          if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
              pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }
            */
        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
     || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
      || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
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
    //    hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
         hbr=(HBRUSH) m_hbrush;

        return hbr;
        break;
      case CTLCOLOR_BTN:
       pDC->SetBkMode(TRANSPARENT);
              hbr=(HBRUSH) m_hbrush;
              return hbr;

        break;
 
    }

	return hbr;
}

BOOL CDlgActionEmail::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
SetBitmap(_T("gui\\background.bmp"));

	m_cst_server = m_p_action->GetEmailServer();

    if (m_cst_server.GetLength() == 0)
    {
        m_cst_server = _T("mail.yourserver.com");
    }

    m_cst_password = m_p_action->GetPassword();
    m_cst_logon = m_p_action->GetLogon();


    m_b_say_count = m_p_action->m_b_say_count;
    m_b_say_from = m_p_action->m_b_say_from;
    m_b_say_subject = m_p_action->m_b_say_subject;

    m_cst_filter_text = m_p_action->m_wst_subject;

    if (m_cst_filter_text.GetLength() < 1)
    {
        m_cst_filter_text = _T("This mail is probably spam.  The original message has been");
        m_p_action->m_i_filter_field = C_FILTER_FIELD_BODY;
    }

    m_i_filter_field = m_p_action->m_i_filter_field;
    m_i_filter_have = m_p_action->m_i_filter_ignore_type;

    m_b_filter_active = m_p_action->m_b_filter_active;
    GrayOutStuff(); 

    UpdateData(D_TO_WINDOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionEmail::GrayOutStuff()
{
     ((CButton*)GetDlgItem(IDC_FILTER_FIELD))->EnableWindow(m_b_filter_active);
     ((CButton*)GetDlgItem(IDC_FILTER_TEXT))->EnableWindow(m_b_filter_active);
     ((CButton*)GetDlgItem(IDC_FILTER_HAVE))->EnableWindow(m_b_filter_active);
   
}

void CDlgActionEmail::OnOK() 
{
	UpdateData(D_TO_VAR);

#ifdef _UNICODE
    m_p_action->SetEmailServer(m_cst_server);
    m_p_action->SetPassword(m_cst_password);
    m_p_action->SetLogon(m_cst_logon);

#else
    m_p_action->SetEmailServer(uni(m_cst_server).us_data);
    m_p_action->SetPassword(uni(m_cst_password).us_data);
    m_p_action->SetLogon(uni(m_cst_logon).us_data);


#endif

    m_p_action->m_b_say_count = m_b_say_count != 0;
    m_p_action->m_b_say_from = m_b_say_from != 0;
    m_p_action->m_b_say_subject = m_b_say_subject != 0;


    //first copy the text over
#ifndef _UNICODE
 m_p_action->SetEmailSubject(uni(m_cst_filter_text).us_data);
#else
 m_p_action->SetEmailSubject(m_cst_filter_text);
#endif

 m_p_action->m_i_filter_field = m_i_filter_field;
 m_p_action->m_i_filter_ignore_type = m_i_filter_have;
 m_p_action->m_b_filter_active = m_b_filter_active != 0;

	CBkDialogST::OnOK();
}

void CDlgActionEmail::OnFilterActive() 
{
	// TODO: Add your control notification handler code here
    UpdateData(D_TO_VAR);
	GrayOutStuff();
}
