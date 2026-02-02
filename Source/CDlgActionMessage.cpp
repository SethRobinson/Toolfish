// CDlgActionMessage.cpp : implementation file
//

#include "cglobals.h"
#include "Source.h"
#include "CDlgActionMessage.h"
#include "..\Shared\uni.h"
#include "CDlgEmailSettings.h"
#include "CAction.h"        
#include "spy_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
                 
/////////////////////////////////////////////////////////////////////////////
// CDlgActionMessage dialog


CDlgActionMessage::CDlgActionMessage(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionMessage)
	m_b_log = FALSE;
	m_b_macro = FALSE;
	m_b_popup = FALSE;
	m_b_say = FALSE;
	m_b_email = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgActionMessage::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionMessage)
	DDX_Control(pDX, IDC_TOKEN, m_but_token);
    DDX_Check(pDX, IDC_B_LOG, m_b_log);
	DDX_Check(pDX, IDC_B_MACRO, m_b_macro);
	DDX_Check(pDX, IDC_B_POPUP, m_b_popup);
	DDX_Check(pDX, IDC_B_SAY, m_b_say);
    DDX_Control( pDX, IDC_RICHEDIT, m_richedit) ;
	DDX_Check(pDX, IDC_B_EMAIL, m_b_email);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionMessage, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionMessage)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_EMAIL, OnEmail)
	ON_BN_CLICKED(IDC_B_EMAIL, OnBEmail)
	ON_BN_CLICKED(IDC_TOKEN, OnToken)
	ON_BN_CLICKED(IDC_HEAR, OnHear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionMessage message handlers




BOOL CDlgActionMessage::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
    //m_richedit.SendMessage(EM_SETOPTIONS,ECOOP_SET, ECO_READONLY );

   SetBitmap(_T("gui\\background.bmp"));
 
   m_but_token.DrawBorder(false, false);
  // m_but_token.SetURL(L"http://www.clear.com")	;
   m_but_token.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(20,20,255), false);
   m_but_token.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0), false);
    m_but_token.DrawTransparent();

    
    if (m_p_action->m_cf.yHeight == 0)
    {
        //set font to default style
        if (app_glo.cf_default_font.yHeight == 0)
        {
            //fill it with the default
            m_richedit.GetDefaultCharFormat(app_glo.cf_default_font);
        }
        m_richedit.SetDefaultCharFormat(app_glo.cf_default_font);
    } else
    {
        //remember what font we were using with this 
		m_richedit.SetDefaultCharFormat(CharFormatAToW(m_p_action->m_cf));
    }
    
    //set the text
         if (m_p_action->GetBufferPointer())
         {
             //something is in here, let's add it
         int len = m_richedit.GetTextLength();
         m_richedit.SetSel(0,len);
#ifdef _UNICODE
         m_richedit.ReplaceSel(m_p_action->GetBufferPointer());

#else
         m_richedit.ReplaceSel((char*)m_p_action->GetBufferPointer());

#endif

         }

     //set check marks
         m_b_log = m_p_action->m_b_log;
         m_b_popup = m_p_action->m_b_popup;
         m_b_say = m_p_action->m_b_say;
         m_b_macro = m_p_action->m_b_macro;
         m_b_email = m_p_action->m_b_email;
         SetGray();

         UpdateData(D_TO_WINDOW);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgActionMessage::SetGray()
{
   ((CButton*)GetDlgItem(IDC_EMAIL))->EnableWindow(m_b_email);
 

}

void CDlgActionMessage::OnFont() 
{
	// TODO: Add your control notification handler code here

CFontDialog dlg;

   if (dlg.DoModal() == IDOK)
   {
   
   
   CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    m_richedit.GetDefaultCharFormat(cf);
    cf.yHeight = dlg.GetSize();
    CString wst_fontname = dlg.GetFaceName();
 

#if _MSC_VER >= 1300

	wcscpy(cf.szFaceName, wst_fontname);

#else
	uni u((LPCTSTR)wst_fontname);
	strcpy(cf.szFaceName, u.to_st());
#endif
 
	m_richedit.SetDefaultCharFormat(cf);


    //set out default font to this for the future
    m_richedit.GetDefaultCharFormat(app_glo.cf_default_font);
   }

}

void CDlgActionMessage::OnOK() 
{
   //actually save our changes
   int len = m_richedit.GetTextLength();
   m_richedit.SetSel(0,len);
   //work around for a CString bug...
   WCHAR *p_buf = new WCHAR[len+1];
   p_buf[len] = 0; //add NULL
  
   m_richedit.GetSelText((char*)p_buf);
   // CString cst_temp = ;
   //wcscpy(p_buf, (const WCHAR*)m_richedit.GetSelText());
   m_p_action->CopyToBuffer(p_buf);
   SAFE_DELETE_ARRAY(p_buf);

   //copy the font
 
   CHARFORMAT cfm;
   m_richedit.GetDefaultCharFormat(cfm);
   
   m_p_action->m_cf = CharFormatWToA(cfm);
   
	 UpdateData(D_TO_VAR);
  
    if (!m_b_log) if (!m_b_macro) if (!m_b_popup) if (!m_b_say) if (!m_b_email)
    {
        int i_res = MessageBox(_T("Hey!  This message won't be shown because you didn't select one of the methods at the bottom. Save anyway?"), _T("Warning"), MB_OKCANCEL | MB_ICONEXCLAMATION);
    if (i_res != IDOK)  return;
  }
   
   
   m_p_action->m_b_log = m_b_log != 0;
   m_p_action->m_b_macro = m_b_macro != 0;
   m_p_action->m_b_popup = m_b_popup != 0;
   m_p_action->m_b_say = m_b_say != 0;
    m_p_action->m_b_email = m_b_email != 0;





	CBkDialogST::OnOK();
}

BOOL CDlgActionMessage::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == 13)
        {
            //they pressed enter, append it manually
           int len = m_richedit.GetTextLength();
         //  m_richedit.G
         //  m_richedit.SetSel(len,len);
#ifdef _UNICODE
           m_richedit.ReplaceSel(_T("\r\n"));
#else
           m_richedit.ReplaceSel((char*)&L"\r\n");

#endif

   return true; //ignore it
        }
    }
    
    return CBkDialogST::PreTranslateMessage(pMsg);
}

HBRUSH CDlgActionMessage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CDlgActionMessage::OnEmail() 
{
	// TODO: Add your control notification handler code here
	DlgEmailSettings dlg;
    dlg.SetActionPointer(m_p_action);
    dlg.DoModal();
}

void CDlgActionMessage::OnBEmail() 
{
	// TODO: Add your control notification handler code here
	UpdateData(D_TO_VAR);
    SetGray();
}

void CDlgActionMessage::OnToken() 
{
 int result = (int)ShellExecute(NULL,_T("open"),_T("docs\\docpages\\token_list.htm"), NULL,NULL, SW_MAXIMIZE);
  ProcessError(this->m_hWnd, result, _T("temp.url"),_T("docs\\docpages\\token_list.htm")); //show an error message if we couldn't open this
	
}

void CDlgActionMessage::OnHear() 
{
    //they want to hear a sample.
    int len = m_richedit.GetTextLength();
   m_richedit.SetSel(0,len);
   //work around for a CString bug...
   WCHAR *p_buf = new WCHAR[len+1];
   p_buf[len] = 0; //add NULL
  
   m_richedit.GetSelText((char*)p_buf);
   // CString cst_temp = ;
   //wcscpy(p_buf, (const WCHAR*)m_richedit.GetSelText());
 
    TCHAR *p_new_buff = NULL;

    //ok, first of all, modify this sucker if we need to

#ifdef _UNICODE 
    TCHAR *p_buf_to_use =  p_buf;
#else

    uni uni_buff(p_buf);
    TCHAR *p_buf_to_use =  uni_buff.st_data;

#endif
     if (!p_buf_to_use)
      {
          LogError(_T("Can't process message, it's blank."));
             SAFE_DELETE_ARRAY(p_buf);
          return; //no message here
      }


     //if a $ is anywhere inside this, let's do our thing on it
     if (wcschr(p_buf, '$') )
     {
       //there is a money symbol in it which means it may need var
       //replacing.  Let's do it.

       //make a buffer for it
         int i_orig_buff_size = wcslen(p_buf);
         int i_buff_size = (i_orig_buff_size *2)+C_MAX_URL_SIZE*2; //to be safe
         p_new_buff = new TCHAR[i_buff_size];

#ifdef _UNICODE
         ReplaceStrings(p_new_buff, p_buf, i_buff_size, i_orig_buff_size, NULL);
#else
         ReplaceStrings(p_new_buff, uni_buff.st_data, i_buff_size, i_orig_buff_size, NULL);

#endif
         p_buf_to_use = p_new_buff;
     }
  if (app_glo.GetSpeech())
        {
          //SAPI won't work in a thread, needs the message pump for messaging
          //Let's send it to our main window to play
          //global say will make a copy and send it
           glo.m_voice.SayTest(p_buf_to_use);
        } else
        {
        LogError(_T("Microsoft SAPI has been disabled from the options menu, can't use text to speech."));
        }

    SAFE_DELETE_ARRAY(p_buf);

   

}
