#include "mail_util.h"

#include "..\Shared\CTextParse.h"
#include "ProcessEvents.h"
//#include "..\Shared\smtp.h"
#include "..\Shared\email\smtp.h"

#include "CGLobals.h"

bool SendEmail(TCHAR *p_content, CEvent *p_event, int i_index)
{
   
	CAction *p_action = p_event->GetAction(i_index);
    TCHAR  ws_temp[MAX_PATH];

    bool b_file_attached = false;

    if (wcslen(p_action->GetEmailFile()) > 2)
    {
        b_file_attached = true;
    }

#ifdef _UNICODE

    if (b_file_attached)
    {
    
    _stprintf(ws_temp, _T("Sending email to %s. (with file attachment)"), p_action->GetToEmail());
    } else
    {
    _stprintf(ws_temp, _T("Sending email to %s."), p_action->GetToEmail());

    }
#else
    if (b_file_attached)
    {
    
    _stprintf(ws_temp, "Sending email to %s. (with file attachment)", uni(p_action->GetToEmail()).to_st());
    } else
    {
    _stprintf(ws_temp, "Sending email to %s.", uni(p_action->GetToEmail()).to_st());

    }
#endif
    
    if (!p_event->GetQuietMode())
        LogMsg(ws_temp);
    
    //Create the message
		  CSMTPMessage message;
          //pMessage->SetCharset(m_sEncodingCharset);
          //CSMTPBodyPart attachment;
          // attachment.SetCharset(m_sEncodingCharset);
          
          message.SetMime(false);
          message.AddMultipleRecipients(p_action->GetToEmail(), CSMTPMessage::TO);
          message.AddTextBody(p_content);
         
  
          message.m_From.m_sEmailAddress.Format(_T("%s"), uni(p_action->GetReturnEmail()).GetAuto());
          
          //easier to work with this way for a few checks
          TCHAR st_temp[256];
          _stprintf(st_temp, uni(p_action->GetReturnEmail()).GetAuto());
          TCHAR *p_email_start, *p_email_end;
          

          if ( (p_email_start = _tcsstr(st_temp, _T("<"))) && (p_email_end = _tcsstr(st_temp, _T(">"))))
          {
              //it appears to have a full email data
              p_email_start++; //move past the <
              p_email_end[0] = 0; //null it before the >

              message.m_ReplyTo.m_sEmailAddress.Format(_T("%s"), p_email_start);

             p_email_start--; //go back to the <
             p_email_start[0] = 0; //null it here so st_temp will only contain the name
             strip_ending_spaces(st_temp);

              message.m_ReplyTo.m_sFriendlyName.Format(_T("%s"), st_temp);

          }   else
          {
          
            //we'll add our own friendly name otherwise some servers will reject this email
          message.m_ReplyTo.m_sEmailAddress.Format(_T("%s"), uni(p_action->GetReturnEmail()).GetAuto());
          message.m_ReplyTo.m_sFriendlyName.Format(_T("%s"), uni("Toolfish Mailer").GetAuto());
          }
          message.m_sSubject.Format(_T("%s"), uni(p_action->GetEmailSubject()).GetAuto());
          message.AddCustomHeader(_T("X-Program: Toolfish"));

if (glo.m_st_smtp_server[0] == 0)
{
	LogError(_T("Can't send mail, you haven't set a smtp server yet.  Check the options!"));
	return false;
}

          //connect to server
          CSMTPConnection connection;
          // if (m_bAutoDial)
          //     connection.ConnectToInternet();
          
          //  BOOL bConnect = connection.Connect(m_sHost, m_Authenticate, m_sUsername, m_sPassword, m_nPort, m_sBoundIP);

 BOOL bConnect;
#ifdef _UNICODE

          if (glo.m_b_smtp_use_auth)
          {
            //use special authentication
              bConnect = connection.Connect(glo.m_st_smtp_server, ((CSMTPConnection::LoginMethod) glo.m_smtp_logon), glo.m_st_smtp_username, 
                glo.m_st_smtp_password, glo.m_i_smtp_port);
          } else
          {
            //regular way
            bConnect = connection.Connect(glo.m_st_smtp_server);
          }
#else
          if (glo.m_b_smtp_use_auth)
          {
              //use special authentication
              bConnect = connection.Connect(uni(glo.m_st_smtp_server).st_data,(CSMTPConnection::LoginMethod)glo.m_smtp_logon, uni(glo.m_st_smtp_username).st_data, 
                  uni(glo.m_st_smtp_password).st_data, glo.m_i_smtp_port);
          } else
          {
              //regular way
              bConnect = connection.Connect(uni(glo.m_st_smtp_server).st_data);
          }    
#endif
   
          
          
          if (!bConnect) 
          {
              LogError(_T("Couldn't connect to server. Error:%d"), GetLastError());
              return false; //error
          }
          else 
          {
             
              //but wait, should we also send a file?
              	if (b_file_attached)
                {
                    //let's do it
#ifdef _UNICODE
                    
                    message.AddMultipleAttachments(p_action->GetEmailFile());		
#else
                    message.AddMultipleAttachments(uni(p_action->GetEmailFile()).st_data);		

#endif
                }
   
              //Send the message
              if (!connection.SendMessage(message)) 
              {
                  LogError(_T("Couldn't send email. Response: %s"), connection.GetLastCommandResponse());
                  return false;
              }
          }
          
#ifdef _UNICODE
          _stprintf(ws_temp, _T("Mail sent to %s."), p_action->GetToEmail());
#else
          _stprintf(ws_temp, "Mail sent to %s.", uni(p_action->GetToEmail()).to_st());
#endif
          
          LogMsg(ws_temp);
          
          return true;
}


//this is funky to take a forced WCHAR because even in a non unicode build it's going
//to send a WCHAR (for saved data compatibility)

TCHAR * FindNoCase(const TCHAR source[], const WCHAR w_text[])
{
#ifdef _UNICODE 
    return stristr(source, w_text);
#else
//we also have to convert it to unicode on this ANSI build
    return stristr(source, uni(w_text).st_data);
#endif

}


bool EmailPassedFilter(CMailMessage * p_mail, CEvent *p_event, int i_index)
{
     CAction *p_action = p_event->GetAction(i_index);
 

     if (!p_action->m_b_filter_active) return true; //automatically passes the email filter, because there isn't one

    bool b_text_found = false;
     //actually scan it.
     if (p_action->m_i_filter_field == C_FILTER_FIELD_SUBJECT)
     {
         if (FindNoCase(p_mail->m_sSubject,p_action->m_wst_subject) != NULL)
             b_text_found = true;

     } else
     if (p_action->m_i_filter_field == C_FILTER_FIELD_BODY)
     {
         
         if (FindNoCase(p_mail->m_sBody,p_action->m_wst_subject) != NULL)
             b_text_found = true;


     } else


     if (p_action->m_i_filter_field == C_FILTER_FIELD_SENDER)
     {
         if (FindNoCase(p_mail->m_sFrom, p_action->m_wst_subject) != NULL)
             b_text_found = true;
     } else
     if (p_action->m_i_filter_field == C_FILTER_FIELD_RECIPIENT)
     {
         //scan through all recipients

         for (int i=0; i < p_mail->GetNumRecipients(); i++)
         {
           CString cst_name;
           CString cst_friendly_name;
           p_mail->GetRecipient(cst_name, cst_friendly_name, i);

           if (FindNoCase(cst_name,p_action->m_wst_subject) != NULL)
             b_text_found = true;
           if (FindNoCase(cst_friendly_name,p_action->m_wst_subject) != NULL)
             b_text_found = true;
         }
         
         
     }
     else
     {
       //error
       LogError(_T("Error in EmailPassedFilter, unknown field type requested."));
       return true;
     }


     if (p_action->m_i_filter_ignore_type == C_FILTER_IGNORE_EMAILS_WITH)
     {
         if (b_text_found) return false;
     }   else
     
     if (p_action->m_i_filter_ignore_type == C_FILTER_IGNORE_EMAILS_WITHOUT)
     {
         if (!b_text_found) return false;
     }   else
     {
         //should never get here.
          LogError(_T("Error in EmailPassedFilter, unknown filter ignore type requested."));
   
     }
     //it passed all tests
     return true;
}


bool ProcessEmail(CEvent *p_event, int i_index)
{
    CAction *p_action = p_event->GetAction(i_index);
   
#ifdef _UNICODE
    CPOP3 pop(p_action->GetEmailServer(), 110,
	 p_action->GetLogon(), p_action->GetPassword());
#else
    CPOP3 pop(uni(p_action->GetEmailServer()).to_st(), 110,
	 uni(p_action->GetLogon()).to_st(), uni(p_action->GetPassword()).to_st());

#endif


  if (!pop.Connect())
  {
    //error connecting?
	  LogMsg(_T("Error connecting to mail server %s. (%s)"), pop.GetServerHostName(), pop.GetLastError());
      	pop.Disconnect();
	  return false; //no mail/error

  }
 
  //first get a file safe version
  CTextParse parse_date;
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
  _stprintf(temp_fname, "%s", uni(p_action->GetLogon()).to_st());
#endif

     CleanFileName(temp_fname);



     #ifdef _UNICODE
  _stprintf(file_name, _T("%s\\%s.dat"), dir_name, temp_fname);
  _stprintf(mailview_file, _T("%s\\%s.tmp"), dir_name, temp_fname);


#else
  _stprintf(file_name, "%s\\%s.dat", uni(dir_name).to_st(), uni(temp_fname).to_st());
_stprintf(mailview_file, _T("%s\\%s.tmp"),uni(dir_name).to_st(), uni(temp_fname).to_st());

#endif


   //create the dir if needed

     BOOL bOK = ::CreateDirectory (dir_name,NULL);
		DWORD	dwLastError = GetLastError();

			if (!bOK && dwLastError == ERROR_PATH_NOT_FOUND)
			{
			
              LogError(_T("Unable to create directory %s."), dir_name);
              return false;
			}

     //now create the name for the email account

  int i_messages = pop.GetNumMessages();

  if (i_messages == -1)
  {
   LogMsg(_T("The mail server %s says it is unable to check now and we should try again later."), pop.GetServerHostName());
	pop.Disconnect();
  	return false; //no mail

  }
   //erase our mailview cache
 //   _tunlink(mailview_file);
    CTextParse parse_mailview;


    WCHAR wst_temp[512];
    swprintf(wst_temp, L"ACCOUNT: %s (server %s)\r\n-------------------------------------------------------------------\r\n",
    p_action->GetLogon(), p_action->GetEmailServer());
    parse_mailview.AddText((char*)&wst_temp, wcslen(wst_temp)*2);
 
  if (i_messages < 1)
  {
     if (!p_event->GetQuietMode())
    log_msg("No new messages available at this time.");
	pop.Disconnect();

          swprintf(wst_temp, L"No emails.  (mailbox empty)\r\n\r\n\r\n");
       parse_mailview.AddText((char*)&wst_temp, wcslen(wst_temp)*2);
     parse_mailview.Save(mailview_file);


    
    
    return false; //no mail
  }
  bool b_counted = false;

  //there are new messages.

  
  if (exist(file_name))
  {
      //load cached thingie
/*
#ifdef _UNICODE
      FILE * fp = _tfopen(file_name, _T("rb"));
#else
      FILE * fp  _tfopen(uni(file_name).to_st(), _T("rb"));
#endif
      //read the time in
      fread(&time_last, sizeof(CTime), 1, fp);
      
      fclose(fp);
      */

#ifdef _UNICODE
      parse_date.open(uni(file_name).to_st());
#else
      parse_date.open(file_name);
#endif

  }

  MailVec v_mail;

  bool b_new = false;
unsigned int i;
  
for (i=0; i < UINT(i_messages); i++)
   {
       //process each message
   //    log_msg("Processing message %d.",i+1);
       CMailMessage *p_msg = new CMailMessage;

       pop.GetMessageTop(i+1, 9, p_msg);
       v_mail.push_back(p_msg);
   }
 
  	pop.Disconnect();

     
 int i_passed_filtered_new = 0;


    //go through each email now
    for (i=0; i < v_mail.size(); i++)
  {
       if (EmailPassedFilter(v_mail[i], p_event, i_index))
       {
          i_passed_filtered_new++; 
        swprintf(wst_temp, L"EMAIL FROM: %s concerning %s\r\n",
        uni(v_mail[i]->m_sFrom).us_data, uni(v_mail[i]->m_sSubject).us_data);
        parse_mailview.AddText((char*)&wst_temp, wcslen(wst_temp)*2);
       }
   }

    if (i_passed_filtered_new == 0)
    {
        //no emails at all.
        if (v_mail.size() == 0)
        {
          swprintf(wst_temp, L"No emails.  (mailbox empty)\r\n\r\n\r\n");
        } else
        {
            //filtering must be active, but no emails were valid.
           swprintf(wst_temp, L"\r\nOf the %d email(s) in the mailbox, none passed your filtering options.\r\n\r\n\r\n",
           v_mail.size());
        }
      
    }   else
    {
           swprintf(wst_temp, L"\r\n%d out of the %d email(s) in the mailbox listed.\r\n\r\n\r\n",
           i_passed_filtered_new, v_mail.size());
    }

  parse_mailview.AddText((char*)&wst_temp, wcslen(wst_temp)*2);
  parse_mailview.Save(mailview_file);


  //first find where the new ones start.
  int i_first_valid_one = 0;
  if (parse_date.GetBuffByteSize() > 5)
  {
      
      for (i=0; i < v_mail.size(); i++)
      {
          if (v_mail[i]->m_sDate.Compare(uni(parse_date.GetBuffer()).GetAuto()) == 0)
              {
                  //log_msg("Found old message at %d.", i);
                  i_first_valid_one = i+1;
              }
              
      }
  }
  
  TCHAR ws_temp[512];
  


int i_say_counter = 0;
int i_total_new = i_messages - i_first_valid_one ;
i_passed_filtered_new = 0;



//scan again, this time against the filter check

   for (i=i_first_valid_one; i < v_mail.size(); i++)
  {
       if (EmailPassedFilter(v_mail[i], p_event, i_index))
       {
          i_passed_filtered_new++; 

          //let's also add this to our super duper check email list
       }
 
  }

#ifdef _DEBUG
  // log_msg("%d messages were filtered and passed out of the %d new.", i_passed_filtered_new, i_total_new);

#endif

if (i_passed_filtered_new > 0)
  {
  
   if (!glo.m_b_tray_no_trigger_on_email)
        SetTrayBlinking(true, 0); //the last number isn't used yet
           
    app_glo.SetLastNewEmailCount(i_total_new);
   LogMsg(_T("%d new message(s) found out of the %d on the server."),i_passed_filtered_new, i_messages);
   if (p_action->m_b_say_count)
   {
  
     if (i_passed_filtered_new == 1)
     {
         _stprintf(ws_temp,_T("Hey, You have received one new e mail."));
     } else
     {
         _stprintf(ws_temp,_T("Hey, You have received %d new e mails."), i_passed_filtered_new);
     }

      GlobalSay(ws_temp);
   }
 
//go through and handle new messages
     for (i=i_first_valid_one; i < v_mail.size(); i++)
  {
        
          TCHAR ws_from[256];
          TCHAR ws_subject[512];

#ifdef _UNICODE

 _tcsncpy(ws_from, v_mail[i]->m_sFrom, 80);
 ws_from[79] = 0; //force null

 _tcsncpy(ws_subject, v_mail[i]->m_sSubject, 128);
 ws_subject[127] = 0; //force null

#else
 _tcsncpy(ws_from, uni(v_mail[i]->m_sFrom).to_st(), 80);
 ws_from[79] = 0; //force null

 _tcsncpy(ws_subject, uni(v_mail[i]->m_sSubject).to_st(), 128);
 ws_subject[127] = 0; //force null

#endif

  if (EmailPassedFilter(v_mail[i], p_event, i_index))
       {
       i_say_counter++;
       if ( (ws_from[0] == 0)
           && (ws_subject[0] == 0))
       {
           //email has no from or subject.. it was probably missing because another email program grabbed it right
           //before we did our final pass.



       }   else
       {
       
 

      LogMsg(_T("NEW EMAIL ARRIVED: #%d is from %s concerning %s."), i_say_counter, ws_from,ws_subject);
      //v_mail[i]->m_tDateTime.Format(_T("%H:%M:%S on %A, %B %d, %Y")), v_mail[i]->m_sDate
      if (p_action->m_b_say_from || p_action->m_b_say_subject)
      {
        
          
          //we need to talk about the emails, but what format?
          if ( (p_action->m_b_say_from) && (!p_action->m_b_say_subject))
          {
              _stprintf(ws_temp,_T("E mail %d is from %s"), i_say_counter, ws_from);
          } 

          if ( (!p_action->m_b_say_from) && (p_action->m_b_say_subject))
          {
              _stprintf(ws_temp,_T("E mail %d is about %s."), i_say_counter, ws_subject);
          }
          TCHAR *p_ws_subject = &ws_subject[0];
          if ( (p_action->m_b_say_from) && (p_action->m_b_say_subject))
          {
              if (
                  ((ws_subject[0] == 'R') || (ws_subject[0] == 'r')) &&
                  ((ws_subject[1] == 'E') || (ws_subject[1] == 'e')) &&
                  (ws_subject[2] == ':')
                  )
              {
                  //remove the first part
                  p_ws_subject = &ws_subject[3];
              }

               if (
                  ((ws_subject[0] == 'F') || (ws_subject[0] == 'f')) &&
                  ((ws_subject[1] == 'W') || (ws_subject[1] == 'w')) &&
                  (ws_subject[2] == ':')
                  )
              {
                  //remove the first part
                  p_ws_subject = &ws_subject[3];
              }


              _stprintf(ws_temp,_T("E mail %d is from %s concerning %s."), i_say_counter, ws_from, p_ws_subject);
          }
                    GlobalSay(ws_temp);
      }
       }
  }
      //save date
#ifdef _UNICODE
      parse_date.ClearBuffer();
      parse_date.AddText(uni(v_mail[i]->m_sDate).to_st(), v_mail[i]->m_sDate.GetLength()+1);  //1 is for the null
#else
      parse_date.ClearBuffer();
      parse_date.AddText((LPCSTR)v_mail[i]->m_sDate,  v_mail[i]->m_sDate.GetLength()+1);
#endif

    }

     //write out latest date info to compare with later
     parse_date.Save(file_name);


  } else
  {
         if (!p_event->GetQuietMode())
         {
         
      if (i_messages == 1) 
      LogMsg(_T("No new emails found. (%d is in the mailbox, but we've already processed it)"),i_messages); else

      LogMsg(_T("No new emails found. (%d are in the mailbox, but we've already processed them)"),i_messages);
         }
 
  }
 
  //delete and free memory
   for (i=0; i < v_mail.size(); i++)
  {
       SAFE_DELETE(v_mail[i]);
  }

   //let's save the date of the last message received
     if (i_say_counter == 0) return false;


    return true;
}
