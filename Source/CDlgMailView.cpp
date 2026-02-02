// CDlgMailView.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgMailView.h"
#include "CGlobals.h"
#include "..\Shared\uni.h"
#include "CEvent.h"
#include "CAction.h" 
#include "CEventList.h"
#include "..\Shared\CTextParse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDlgMailView dialog


CDlgMailView::CDlgMailView(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgMailView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMailView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgMailView::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMailView)
  DDX_Control( pDX, IDC_RICHEDIT, m_richedit) ;

		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMailView, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgMailView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMailView message handlers

HBRUSH CDlgMailView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgMailView::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
   SetBitmap(_T("gui\\background.bmp"));
   m_richedit.AutoURLDetect(true);
   m_richedit.SetEventMask(m_richedit.GetEventMask () | ENM_LINK);
 	
   BuildText();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgMailView::ProcessMailBox(CEvent *p_event, int i_action)
{
 
      CAction *p_action = p_event->GetAction(i_action);
  
    TCHAR  file_name[MAX_PATH];
     TCHAR  temp_fname[MAX_PATH];
     TCHAR mailview_file[MAX_PATH];


   #ifdef _UNICODE
  _stprintf(temp_fname, _T("email_%s"), p_action->GetEmailServer());

#else
  _stprintf(temp_fname, "email_%s", uni(p_action->GetEmailServer()).to_st());
#endif

   CleanFileName(temp_fname);

  TCHAR dir_name[512];
#ifdef _UNICODE
  _stprintf(dir_name, _T("cache\\%s"), temp_fname);

#else
  _stprintf(dir_name, "cache\\%s", uni(temp_fname).to_st());
#endif

  //create the final file name

   #ifdef _UNICODE
  _stprintf(temp_fname, _T("%s"), p_action->GetLogon());

#else
  _stprintf(temp_fname, "%s", uni(p_action->GetLogon()).st_data);
#endif

     CleanFileName(temp_fname);
        #ifdef _UNICODE
  _stprintf(file_name, _T("%s\\%s.dat"), dir_name, temp_fname);
  _stprintf(mailview_file, _T("%s\\%s.tmp"), dir_name, temp_fname);


#else
  _stprintf(file_name, "%s\\%s.dat", uni(dir_name).st_data, uni(temp_fname).st_data);
_stprintf(mailview_file, _T("%s\\%s.tmp"),uni(dir_name).st_data, uni(temp_fname).st_data);

#endif

 //now let's get down to business.
 uni uni_mail(mailview_file);

 //I do it this way because if parse.open fails it writes a line to the main window
 //log which isn't what I want

 CTextParse parse;
 if ( (!exist(uni_mail.st_data) || (!parse.open(uni_mail.st_data)) )   )
 {
     WCHAR wst_temp[512];
     swprintf(wst_temp, L"No data found for %s, account %s, try again later.\r\n\r\n\r\n", p_action->GetEmailServer(),
         p_action->GetLogon());
     AddLineOfText(wst_temp);
 }  else
 {
     //add the real info we just loaded.
     AddLineOfText((WCHAR*)parse.GetBuffer());

 }


}


bool ColorizeWordMail(CRichEditCtrlEx * p_rich_edit, CHARRANGE *p_range, const WCHAR *p_line, WCHAR *p_word, COLORREF color, bool b_bold)
{
    
   // OutputDebugString(p_line);
    WCHAR *p_first_char;
    if ( p_first_char = (WCHAR*)wcsstr(p_line, p_word))
    {
    
         int i_index = p_first_char-p_line;
   
        //found the word.
        CHARFORMATW cfColor; //force unicode version, even on ANSI build
        ZeroMemory(&cfColor, sizeof(CHARFORMATW));
        cfColor.cbSize =sizeof(CHARFORMATW);
        
        //the color we want
        cfColor.dwMask = CFM_COLOR;
        cfColor.crTextColor = color;
            
            if (b_bold)
            {
                cfColor.dwMask = CFM_COLOR|CFM_BOLD;
                cfColor.dwEffects = CFE_BOLD;
            }
        //    char st_crap[1024];
        //    sprintf(st_crap, "min is %d, max is %d.", p_range->cpMin+i_index, p_range->cpMin+i_index+wcslen(p_word));
        //    log_error(st_crap);

             //move the selection a bit
           p_rich_edit->SetSel(p_range->cpMin+i_index, p_range->cpMin+i_index+wcslen(p_word));
          
//            if (!p_rich_edit->SetSelectionCharFormat((CHARFORMATA)cfColor))

            //color it
            if (p_rich_edit->SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, ( LPARAM ) &cfColor ) == 0)
            {
                LogError(_T("Error setting color."));
            }
        //remove selection so we don't see that annoying box
            //p_rich_edit->SetSel();
            
    }
    

    return true;
}

void ColorizeLineMail(CRichEditCtrlEx * p_rich_edit, int i_line_to_color)
{
     
 #ifdef _UNICODE 
    GETTEXTLENGTHEX gtlx = { GTL_NUMCHARS, CP_THREAD_ACP };
#else
    GETTEXTLENGTHEX gtlx = { GTL_NUMCHARS, CP_THREAD_ACP };
    
#endif
    
  
   int i_total_length = p_rich_edit->SendMessage(EM_GETTEXTLENGTHEX, (WPARAM) &gtlx, 0);
   
    int i_start_position;
    int i_end_position;
    
    if (i_line_to_color+1 == p_rich_edit->GetLineCount())
    {
        //special code for handling the last line...
           i_start_position = p_rich_edit->LineIndex(i_line_to_color-1);
           i_end_position = i_total_length;

    }
    else
    {
    
 
    i_start_position = p_rich_edit->LineIndex(i_line_to_color);
    i_end_position = i_start_position + p_rich_edit->LineLength(i_line_to_color);

    }

    CHARRANGE range;
    range.cpMin = i_start_position;
    range.cpMax = i_end_position;
    
    p_rich_edit->SetSel(range);

    WCHAR st_temp[1024];

    p_rich_edit->GetSelText((char*)st_temp);

    //do our coloring thing
    ColorizeWordMail(p_rich_edit, &range, st_temp, L"No emails.", RGB(255,0,0), true);
    ColorizeWordMail(p_rich_edit, &range, st_temp, L"ACCOUNT:", RGB(0,170,0), true);
    ColorizeWordMail(p_rich_edit, &range, st_temp, L"EMAIL FROM:", RGB(160,0,255), true);
  
}



void CDlgMailView::BuildText()
{

   //scan for check email events
      CEvent *p_event = NULL;
      int i_mailboxes_processed = 0;
      
      for (int i_event=0; i_event < app_glo.GetEventList()->GetCount(); i_event++)
      {
          if (app_glo.GetEventList()->GetEvent(i_event))
          {
              p_event = app_glo.GetEventList()->GetEvent(i_event);
              
              if (!p_event->GetEnabled()) continue; //event disabled  
              
                for (int i=0; i < p_event->GetActionCount(); i++)
              {
                  if (p_event->GetAction(i))
                  {
                      
                      if (p_event->GetAction(i)->GetActionType() == C_ACTION_EMAIL)
                      {
                         ProcessMailBox(p_event, i);
                         i_mailboxes_processed++;
                      }
                  }
                }

          }
      }

     if (i_mailboxes_processed == 0)
     {
         AddLineOfText(L"No active mail events exist.  Use the Event Wizard to create one!\r\n\r\n");
         AddLineOfText(L"Features of Toolfish's POP3 email check features:\r\n\r\n");
         AddLineOfText(L"* Doesn't remove email from the server, just tells you about it\r\n");
         AddLineOfText(L"* Supports voice announcements using text to speech technology\r\n");
         AddLineOfText(L"* Filtering options, support for ISP based anti-spam systems such as SpamAssassin\r\n");
         AddLineOfText(L"* Smart check system won't announce the same email twice\r\n");
         AddLineOfText(L"* No reason to keep a memory hog like Outlook running, Toolfish's memory footprint is tiny!\r\n");
         AddLineOfText(L"* Supports monitoring of unlimited servers/accounts\r\n");
         AddLineOfText(L"* Able to do things like email your cellphone if an email from a certain person is received\r\n");
     }

  //colorize the text all cool
   for (int i=0; i < m_richedit.GetLineCount(); i++)
          {
              ColorizeLineMail(&m_richedit, i);
          }

}


void CDlgMailView::AddLineOfText(WCHAR *p_w_text)
{
    int len = m_richedit.GetTextLength();
         m_richedit.SetSel(len,len);
     #ifdef _UNICODE
         m_richedit.ReplaceSel(p_w_text);
#else
         m_richedit.ReplaceSel((char*)p_w_text);
#endif

}

LRESULT CDlgMailView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
