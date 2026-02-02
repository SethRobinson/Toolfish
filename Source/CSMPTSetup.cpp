// CSMPTSetup.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CGLobals.h"

#include "CSMPTSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMPTSetup dialog


CSMPTSetup::CSMPTSetup(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CSMPTSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMPTSetup)
	m_cst_email = _T("");
	m_cst_server = _T("");
	m_b_logon = FALSE;
	m_b_mime = FALSE;
	m_cst_password = _T("");
	m_cst_username = _T("");
	m_i_port = 0;
	//}}AFX_DATA_INIT
}


void CSMPTSetup::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMPTSetup)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Text(pDX, IDC_DEFAULT_EMAIL, m_cst_email);
	DDV_MaxChars(pDX, m_cst_email, 254);
	DDX_Text(pDX, IDC_SERVER, m_cst_server);
	DDV_MaxChars(pDX, m_cst_server, 254);
	DDX_Check(pDX, IDC_LOGON, m_b_logon);
	DDX_Check(pDX, IDC_MIME, m_b_mime);
	DDX_Text(pDX, IDC_PASSWORD, m_cst_password);
	DDX_Text(pDX, IDC_USERNAME, m_cst_username);
	DDX_Text(pDX, IDC_PORT, m_i_port);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMPTSetup, CBkDialogST)
	//{{AFX_MSG_MAP(CSMPTSetup)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_LOGON, OnLogon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMPTSetup message handlers

HBRUSH CSMPTSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
     || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
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

void CSMPTSetup::BuildComboBox()
{
    m_combo.ResetContent();

    int i_index;
    
    i_index = m_combo.AddString(_T("AUTH LOGIN"));
    m_combo.SetItemData(i_index, CConfig::AuthLoginMethod);

    i_index = m_combo.AddString(_T("CRAM MD5"));
    m_combo.SetItemData(i_index, CConfig::CramMD5Method);

    i_index = m_combo.AddString(_T("LOGIN PLAIN"));
    m_combo.SetItemData(i_index, CConfig::LoginPlainMethod);

}

BOOL CSMPTSetup::OnInitDialog() 
{
    CBkDialogST::OnInitDialog();
    
    SetBitmap(_T("gui\\background.bmp"));
    
    BuildComboBox();
    
    //enter defaults
#ifdef _UNICODE
    m_cst_email = glo.m_st_email;
    m_cst_server = glo.m_st_smtp_server;
    m_cst_username = glo.m_st_smtp_username;
    m_cst_password = glo.m_st_smtp_password;
#else
    
    m_cst_email = uni(glo.m_st_email).to_st();
    m_cst_server = uni(glo.m_st_smtp_server).to_st();
    m_cst_username = uni(glo.m_st_smtp_username).to_st();;
    m_cst_password = uni(glo.m_st_smtp_password).to_st();;
#endif
    
    m_b_logon = glo.m_b_smtp_use_auth;
    m_b_mime = glo.m_b_smtp_mime;
    m_i_port = glo.m_i_smtp_port;
    set_selection_by_data(&m_combo, glo.m_smtp_logon);
    SetGrayStuff();
    UpdateData(D_TO_WINDOW);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CSMPTSetup::SetGrayStuff()
{

  ((CEdit*)GetDlgItem(IDC_PASSWORD))->EnableWindow(m_b_logon);
  ((CEdit*)GetDlgItem(IDC_USERNAME))->EnableWindow(m_b_logon);
  ((CComboBox*)GetDlgItem(IDC_COMBO))->EnableWindow(m_b_logon);

}


void CSMPTSetup::OnOK() 
{
UpdateData(D_TO_VAR);

#ifdef _UNICODE
    wcscpy(glo.m_st_email, m_cst_email);
    wcscpy(glo.m_st_smtp_server, m_cst_server);
    wcscpy(glo.m_st_smtp_username, m_cst_username);
    wcscpy(glo.m_st_smtp_password, m_cst_password);

#else
    wcscpy(glo.m_st_email, uni(m_cst_email).us_data);
    wcscpy(glo.m_st_smtp_server, uni(m_cst_server).us_data);
    wcscpy(glo.m_st_smtp_username,  uni(m_cst_username).us_data);
    wcscpy(glo.m_st_smtp_password,  uni(m_cst_password).us_data);

#endif
   glo.m_b_smtp_mime = m_b_mime != 0;
   glo.m_b_smtp_use_auth = m_b_logon != 0;

   glo.m_smtp_logon = (CConfig::LoginMethod)m_combo.GetItemData(m_combo.GetCurSel());
   glo.m_i_smtp_port = m_i_port;
	CBkDialogST::OnOK();
}

void CSMPTSetup::OnLogon() 
{
  UpdateData(D_TO_VAR);
  SetGrayStuff();
  if (!m_b_logon)
  {
      InvalidateRect(NULL);
  }
}
