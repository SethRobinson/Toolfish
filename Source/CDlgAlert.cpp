// CDlgAlert.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgAlert.h"
#include "CGlobals.h"
#include "..\Shared\uni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlert dialog


CDlgAlert::CDlgAlert(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgAlert::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlert)
	//}}AFX_DATA_INIT

   m_p_action = NULL;
   m_cst_window_caption.Format(_T("Toolfish Notification"));
}


void CDlgAlert::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlert)
     DDX_Control( pDX, IDC_RICHEDIT, m_richedit) ;

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlert, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgAlert)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlert message handlers

HBRUSH CDlgAlert::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
	
switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
    
             // log_msg("Group box detected.");
            pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) m_hbrush;
            return hbr;
      
      default:
        
        //pDC->SetTextColor(MY_TEXTCOLOR);
        pDC->SetBkMode(TRANSPARENT);
     //  hbr=(HBRUSH) GetStockObject(BLACK_BRUSH);
      hbr=(HBRUSH) m_hbrush;
       return hbr;
        break;
      
    }
	return hbr;

}

void CDlgAlert::StoreTextPointerForLater(const TCHAR *p_ws_text)
{
    m_p_ws_text = (TCHAR*)p_ws_text;
}
void CDlgAlert::SetText(WCHAR *p_ws_text)
{
         int len = m_richedit.GetTextLength();
         m_richedit.SetSel(0,len);
     #ifdef _UNICODE
         m_richedit.ReplaceSel(p_ws_text);
#else
         m_richedit.ReplaceSel((char*)p_ws_text);
#endif



}

BOOL CDlgAlert::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
   m_richedit.AutoURLDetect(true);
   m_richedit.SetEventMask(m_richedit.GetEventMask () | ENM_LINK);
   SetWindowText(m_cst_window_caption);
 //m_richedit.SendMessage(EM_SETWORDBREAKPROC, WBF_WORDBREAK, 0);

   //setup font info if we have it
   if (m_p_action)
   {
       
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
       
   }
   

#ifdef _UNICODE
     SetText(m_p_ws_text);
#else

     SetText(uni(m_p_ws_text).us_data);
#endif
  

  //  m_richedit::CRichEditCtrl::SetTe
	 //ShowWindow(SW_NORMAL);
	 //BringWindowToTop();
	 ::SetForegroundWindow(GetSafeHwnd());
	
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlert::OnOK() 
{
	// TODO: Add extra validation here
	
	CBkDialogST::OnOK();
}

LRESULT CDlgAlert::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    if (message == WM_NOTIFY)
    {
       LPNMHDR lpNmhdr = (LPNMHDR)lParam;
            switch ( lpNmhdr->code)
            {
            case  EN_LINK:
                {
                    ENLINK *pEnLink = (ENLINK *)lParam;
                    
                    if(pEnLink->msg == WM_LBUTTONUP)
                    {
                        WCHAR str[C_MAX_URL_SIZE];


                        TEXTRANGEW tr;
                        tr.chrg.cpMin = pEnLink->chrg.cpMin;
                         tr.chrg.cpMax = pEnLink->chrg.cpMax;
                         tr.lpstrText = str;
                         m_richedit.SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
        #ifdef _UNICODE
                         LaunchURL(str);
#else
                         LaunchURL(uni(str).to_st());
#endif
       
                         //stop the annoying blinking
                         SetTrayBlinking(false, 0); //the last number isn't used yet
                         /*
                         winall_create_url_file_full(uni(str).to_st());
               
                         int result = (int)ShellExecute(NULL,_T("open"),_T("temp.url"), NULL,NULL, SW_MAXIMIZE);
#ifdef _UNICODE
                         ProcessError(this->m_hWnd, result, _T("temp.url"),str); //show an error message if we couldn't open this
#else
                         ProcessError(this->m_hWnd, result, _T("temp.url"),uni(str).to_st()); //show an error message if we couldn't open this

#endif
                        */

                    }
                }	
            }
    }
	
	return CBkDialogST::WindowProc(message, wParam, lParam);
}
