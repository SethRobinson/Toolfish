#include "stdafx.h"
#include "internet_util.h"
#include "..\Shared\CTextParse.h"
#include "..\Shared\winsock\winsock_util.h"
#include "..\Shared\winsock\RawSocket.h"
#include "spy_util.h"
                         
#include <wininet.h>

void RefreshCache(CTextParse *p_parse, TCHAR * p_ws_domain, TCHAR *p_ws_filename)
{
   //if the dir doesn't exist, let's make it
     	DWORD dwLastError;
		BOOL bOK;
	    TCHAR ws_temp[MAX_PATH];
        _stprintf(ws_temp, _T("cache\\%s"), p_ws_domain);
  
        bOK = ::CreateDirectory (ws_temp,NULL);
			dwLastError = GetLastError();

			if (!bOK && dwLastError == ERROR_PATH_NOT_FOUND)
			{
			
              LogError(_T("Unable to create directory %s."), ws_temp);
              return;
			}
		
    //write out the cached file
     _stprintf(ws_temp, _T("cache\\%s\\%s"), p_ws_domain, p_ws_filename);
     p_parse->Save(ws_temp);

}


//p_original_url and p_full should both be the same when sending them in?
bool BreakURLDown(const TCHAR *p_original_url, TCHAR *p_full, TCHAR * p_ws_domain, TCHAR * p_ws_path,
                  TCHAR * p_ws_filename)
{

    if (_tcsncicmp(p_full, _T("http://"),_tcslen(_T("http://"))) == 0)
    {
        //get rid of it!
        p_full = (TCHAR*)&p_original_url[_tcslen(_T("http://"))];
    }
     
       
        int i_domain_end_index = _tcscspn(p_full, _T("/\\"));
        
             _tcsncpy(p_ws_domain, p_full, i_domain_end_index);
            p_ws_domain[i_domain_end_index] = 0; //add null
            
            //now get the rest of the url
            //now get just the filename
            if (i_domain_end_index == int(_tcslen(p_full)))
            {
                //it's a domain name only
                _tcscpy(p_ws_path, _T(""));
                _tcscpy(p_ws_filename, _T("index.htm")); //we have to save it to our cache with some kind of name...
            } else
            {
            _tcscpy(p_ws_path, p_full+i_domain_end_index+1); //plus 1 is to grab the null too
                //find the filename at the end of the path
                
            int i_first_occurance = _tcscspn(p_ws_path, _T("/=?"));
            
            
            TCHAR *p_start_of_filename = NULL;
            if (i_first_occurance != 0) p_start_of_filename = &p_ws_path[i_first_occurance];
                
                
                
                
                if (!p_start_of_filename)
                {
                    //no slash was found. this is it.
                    
                    if (_tcslen(p_ws_path) > 0)
                    {
                        _tcscpy(p_ws_filename, p_ws_path);
                    }   else
                    {
                        //  LogMsg(L"Couldn't extract filename to save as from URL, using unknown.htm.");
                        _tcscpy(p_ws_filename, _T("unknown.htm")); //we have to save it to our cache with some kind of name...
                    }
                    
                    
                } else
                {
                    
                    
                    
                    if (p_start_of_filename[0])
                    {
                        p_start_of_filename++;
                    }
                    
                    
                    if ( (!p_start_of_filename[0]))
                    {
                        //there was no slash found, but this still could be the correct file name..
                        int i_file_name_length = 0;
                        if ( (i_file_name_length =_tcslen(p_ws_path)) > 0)
                        {
                           
                            _tcsncpy(p_ws_filename, p_ws_path, 80);
                            p_ws_filename[80] = 0; //null just in case we had truncated it
                        }   else
                        {
                            //  LogMsg(L"Couldn't extract filename to save as from URL, using unknown.htm.");
                            _tcscpy(p_ws_filename, _T("unknown.htm")); //we have to save it to our cache with some kind of name...
                        }
                    }   else
                    {
                        //copy it
                        //let's check the length
                       
                        _tcsncpy(p_ws_filename, p_start_of_filename, 80); //the plus 1 is to skip the slash
                             p_ws_filename[80] = 0; //null just in case we had truncated it
                      
                    }
                }
            }
            
            return true;  //success
}



bool ProcessCompare( CEvent *p_event, int i_index)
{
    CAction *p_action = p_event->GetAction(i_index);
    //log_msg("Checking website %s.", uni(p_action->GetFilename()).to_st());

    //first, break off the domain name and use it to make the directory

    //we need three things to do this.  the .com only, the full path and file, and just the file.

    //info
    
    bool b_trigger_activated = false;
    float f_change = 0; //how much the file has changed by (might not be used if not doing a content compare)
    

    bool b_had_word = false;
    bool b_missing_word = false;
    bool b_first_time_to_cache = false;
    bool b_percent_trigger = false;

    TCHAR wst_had_word[256];
    TCHAR wst_missing_word[256];
    
    TCHAR ws_domain[C_MAX_URL_SIZE];
    TCHAR ws_path[C_MAX_URL_SIZE];
    TCHAR ws_filename[C_MAX_URL_SIZE];

    TCHAR ws_date_updated[256];
    bool b_date_triggered = false;

    char * p_new_date = "Unknown";
    char * p_old_date = "Unknown";

    TCHAR p_full[C_MAX_URL_SIZE];
    TCHAR ws_full_url[C_MAX_URL_SIZE];
  
  
    uni uni_filename(p_action->GetFilename());
  
    ReplaceStrings(ws_full_url, uni_filename.GetAuto(), C_MAX_URL_SIZE, wcslen( (WCHAR*)uni_filename.us_data), NULL);
     
    //get rid of the starting http if applicapable

    _tcscpy(p_full, ws_full_url);

    if (!BreakURLDown(ws_full_url, p_full, ws_domain, ws_path, ws_filename))
   {
       LogError(_T("Error understaing URL, maybe you typed it wrong? (%s), "), uni_filename.GetAuto());
       return true;
   }

    app_glo.SetLastURLCompared(p_full);
    
        
        //   log_msg("Domain index is %s. Path is %s. Filename will be %s.", uni(ws_domain).to_st(), uni(ws_path).to_st(), uni(ws_filename).to_st());
        
        
        //get the file and header info
        CTextParse new_content;
        CWebHeader new_header;
        if (GetWebFile(uni(ws_domain).to_st(), uni(ws_path).to_st(), &new_content, &new_header, p_action->GetWebCompareAuthenticate(),
            p_action->GetLogon(), p_action->GetPassword()))
        {
            //got it!
            //now we need to know if it is chanegd enough to trigger future actions
            CleanFileName(ws_domain);
            CleanFileName(ws_filename);
            
            
            //first let's do the easy one, if the older file doesn't to compare it with, well, it's new
            //create pathname for the saved file
            TCHAR ws_old_file[MAX_PATH];
            _stprintf(ws_old_file, _T("cache\\%s\\%s"), ws_domain, ws_filename);
            // log_msg("Does %s exist?", uni(ws_old_file).to_st() );
            
            
            //first the obvious check
            if (!exist(ws_old_file))
            {
                //nothing to compare it to
                //log_msg("Nothing to compare it to, caching current version for future comparisons. (%s)", uni(p_action->GetFilename()).to_st());
                b_first_time_to_cache = true;
            } 
            
            
            CTextParse old_content;
            
            if (!b_first_time_to_cache)
            {
                
                //load old file
                old_content.open(uni(ws_old_file).to_st());
                
                if (p_action->m_b_date)
                {
                    //has the date changed at all?
                    
                    p_new_date = new_content.GetPointerToFirstLineStartingWith("Last-Modified: ");
                    if (!p_new_date)
                    {
                        LogMsg(_T("Web Compare: %s does not contain last modified data.  Date compare info ignored."), p_full);
                        
                    }  else
                    {
                        p_old_date = old_content.GetPointerToFirstLineStartingWith("Last-Modified: ");
                        if (!p_old_date)
                        {
                            LogMsg(_T("Web Compare: Cached version of %s does not contain last modified data.  Date compare info ignored."), p_full);
                        }
                        //    log_msg("Comparing %s to %s.", p_new_date, p_old_date);
                        //compare the dates, are they the same?  Let's do a real simple string compare for now...
                        if (stricmp(p_new_date, p_old_date) != 0)
                        {
                            //date changed!
                            //cashe new version
                            b_date_triggered = true;
                            b_trigger_activated = true;
                            _tcscpy(ws_date_updated, uni(p_new_date).GetAuto());
                        } else
                        {
                            //log_error("Web Compare: %s's date has not changed.  (%s)",  uni(ws_filename).to_st(), p_new_date);
                            
                        }
                    }
                }
            }  
            
            if (!b_first_time_to_cache)
            {
                
                if (p_action->m_b_content)
                {
                    //actually compare the content, this is the most tricky one...
                    char * p_new_data = new_content.GetBufferAtWord("Content-Type:");
                    if (!p_new_data)
                    {
                        //start from the top since COntent-Type is missing, this is our backup plan
                        p_new_data = new_content.GetBuffer();
                    // LogError(L"Web Compare: %s - Unable to get Content-Type from webpage header.  Our fault our theirs?  Not sure.", p_full);
                        //return false;
                    }
                    
                    char * p_old_data = old_content.GetBufferAtWord("Content-Type:");
                    
                    if (!p_old_data)
                    {
                       
                             p_old_data = old_content.GetBuffer();
                   
                   // LogError(L"Web Compare - Unable to get Content-Type from cached webpage header.  Ignoring content check.");
                        
                    }   else
                    {
                        
                        
                        //let's compare
                        
                        f_change = GetPercentOfChangeBetweenTexts(p_old_data, p_new_data);
                        
                        if (f_change+0.005f >= p_action->m_f_content_percent)
                        {
                            b_trigger_activated = true;
                            b_percent_trigger = true;
                        }
                    }
                }    
            }
            
                }   else
            {
                    LogMsg(_T("Web Compare: Failed to get webfile, unable to do compare. (%s)"), p_full);
                    return false;
                }
                
                
                
                
                if (!b_first_time_to_cache)
                {
                    
                    if (p_action->m_b_words_found)
                    {
                        //if any of these comma delimanated (i don't care if it's spelled wrong, bite me) words are found
                        //trigger it.
                        char seps[]   = ",";
                        char *token;
                        
                        uni un_words(p_action->m_ws_words_found); //convert from unicode, our buffer is in ANSI
                        
                        token = strtok( un_words.st_data, seps );
                        while( token != NULL )
                        {
                            /* While there are tokens in "string" */
                            if (new_content.GetBufferAtWord(token))
                            {
                                //word found.
                                
                                //copy word to buffer for later
                                _tcscpy(wst_had_word, uni(token).GetAuto());
                                b_had_word = true;
                                b_trigger_activated = true;
                                break;
                            }
                            /* Get next token: */
                            token = strtok( NULL, seps );
                        }
                    }
                    
                }
                
                if (!b_first_time_to_cache)
                {
                    
                    if (p_action->m_b_words_not_found)
                    {
                        //if any of these comma delimanated (i don't care if it's spelled wrong, bite me) words are found
                        //trigger it.
                        char seps[]   = ",";
                        char *token;
                        
                        uni un_words(p_action->m_ws_words_not_found); //convert from unicode, our buffer is in ANSI
                        
                        token = strtok( un_words.st_data, seps );
                        while( token != NULL )
                        {
                            /* While there are tokens in "string" */
                            if (new_content.GetBufferAtWord(token))
                            {
                                
                            }   else
                            {
                                //required word not found
                                _tcscpy(wst_missing_word, uni(token).GetAuto());
                                b_missing_word = true;
                                b_trigger_activated = true;
                                break; 
                            }
                            /* Get next token: */
                            token = strtok( NULL, seps );
                        }
                    }
                    
                }
                if (!p_action->m_b_content) if (!p_action->m_b_date) if (!p_action->m_b_words_found) if (!p_action->m_b_words_not_found)
                {
                    b_trigger_activated = true;  
                } 
                if (b_first_time_to_cache)
                {
                    b_trigger_activated = true;
    }
    
    if (b_trigger_activated)
    {
        if (!glo.m_b_tray_no_trigger_on_webcompare)
        SetTrayBlinking(true, 0); //the last number isn't used yet
        //let's build what happened
        TCHAR st_reason[256];
        st_reason[0] = 0;
        if (b_first_time_to_cache)
        {
            _stprintf(st_reason, _T("First time run.  Caching page for future comparisons."));
        }   else
            
            if (b_percent_trigger)
            {
                _stprintf(st_reason, _T("Page content changed %.0f%% (%.0f%% required)"), f_change*100, p_action->m_f_content_percent*100);
            }  else
                if (b_had_word)
                {
                    _stprintf(st_reason, _T("Keyword \"%s\" was found."), wst_had_word);
                } else
                    if (b_missing_word)
                    {
                        _stprintf(st_reason, _T("Required keyword \"%s\" was NOT found."), wst_missing_word);
                    } else
                        
                        if (b_date_triggered)
                        {
#ifdef _UNICODE
                            _stprintf(st_reason, _T("Page filedate has changed to %s. (cached version was %s)"), uni(p_new_date).us_data, uni(p_old_date).us_data);
#else
                            _stprintf(st_reason, _T("Page filedate has changed to %s. (cached version was %s)"), p_new_date, p_old_date);

#endif
                        } else
                            
                            
                            
                            
                        {
                            //default, no info
                            _stprintf(st_reason, _T("(no compare options selected, this will always trigger)"));
                            
                        }
                        
                        
                        //trigged text

                        LogMsg(_T("PAGE TRIGGERED: %s (%s b) %s"), p_full, int_to_cash(int(new_content.GetBuffByteSize())),
                            st_reason);

                        
    } else
    {
        
        //add extra info to help people understand why the page update hasn't taken place.
        TCHAR wst_content[256] = _T("");
        TCHAR wst_keywords[256] = _T("");
        TCHAR wst_date[256] = _T("");
        
        
        if (p_action->m_b_content)
        {
            _stprintf(wst_content, _T("(%.0f%% change (%.0f%% required)) "), f_change*100, p_action->m_f_content_percent*100);
        }
        
        if ( (p_action->m_b_words_found ) || (p_action->m_b_words_not_found) )
        {
            _stprintf(wst_keywords, _T("(phrase check) "));
        }
        if (p_action->m_b_date)
        {
#ifdef _UNICODE
            _stprintf(wst_date, _T("(%s) "), uni(p_new_date).us_data);
#else
            _stprintf(wst_date, _T("(%s) "), p_new_date);

#endif
        } 
        
        if (!p_event->GetQuietMode())
        {
            
            //not triggered text
#ifdef _UNICODE
            LogMsg(_T("Page not triggered. %s%s%s (%s) (%s b)"),  
                wst_content, wst_keywords, wst_date, p_action->GetFilename(), int_to_cash(int(new_content.GetBuffByteSize())));
            
#else
            LogMsg(_T("Page not triggered. %s%s%s (%s) (%s b)"),  
                wst_content, wst_keywords, wst_date, uni(p_action->GetFilename()).to_st(), int_to_cash(int(new_content.GetBuffByteSize())));
            
#endif
            
        }
        
    }
    
  
    if ( (b_trigger_activated) || (b_first_time_to_cache) )
    {
           RefreshCache(&new_content, ws_domain, ws_filename);
   
    }
    return b_trigger_activated; //not updated
}




bool GetWebFile(const char * p_st_server, const char *p_st_file, CTextParse *p_parse, CWebHeader *p_header, const bool b_authenticate, const WCHAR *p_wst_logon, const WCHAR *p_wst_pass)
{
    //get rid of the starting http if we have it..

    char st_server[C_MAX_URL_SIZE]; //temp work buffer
    sprintf(st_server, p_st_server);
    
    DWORD dwConnectionTypes = INTERNET_CONNECTION_LAN |
        INTERNET_CONNECTION_MODEM |
        INTERNET_CONNECTION_PROXY;
    if (!InternetGetConnectedState(&dwConnectionTypes, 0))
    {
        InternetAutodial(INTERNET_AUTODIAL_FORCE_UNATTENDED,
            0);
    } 
   //  log_msg("Connecting to %s.", st_server);		
    struct sockaddr_in blah;
    struct hostent *he;
    memset ((char *) &blah,'0', sizeof(blah));
    
    if ((he = gethostbyname(st_server)) != NULL)
    {
        //change it to numbers
        
        memcpy((char *) &blah.sin_addr, he->h_addr, he->h_length );
        sprintf(st_server, "%s", inet_ntoa(blah.sin_addr));
        
   // log_msg("Converted to %s.", st_server);		
    }
    
    
    CWizReadWriteSocket socket;
    
    if(!socket.Connect(uni(st_server).GetAuto(), 80))
    {
        log_msg("Cannot connect to server");
        return false;
    }
    
    char st_temp[C_MAX_URL_SIZE];
    char st_buffer[C_MAX_URL_SIZE];
    st_buffer[0] = 0;

    sprintf(st_temp, "GET /%s HTTP/1.0\r\nAccept: */*\r\n",  p_st_file);
    strcat(st_buffer, st_temp);
    
 //  strcat(st_buffer, "Accept-Language: en-us\r\n");
 // strcat(st_buffer, "Accept-Encoding: gzip, deflate\r\n");
 // strcat(st_buffer, "Connection: Keep-Alive\r\n");
    sprintf(st_temp, "Host: %s\r\n",p_st_server);
    strcat(st_buffer, st_temp);
  //  strcat(st_buffer, "Connection: Close\n");
    strcat(st_buffer, "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1;)\r\n");
   if (b_authenticate) 
   {
    
    CString cst_format;
    cst_format.Format(_T("%s:%s"), uni(p_wst_logon).GetAuto(),uni(p_wst_pass).GetAuto());
       //For basic authentication
    CString cst_encoded = EncodeTextBase64(cst_format);
    uni u(cst_encoded);

    sprintf(st_temp, "Authorization: Basic %s\r\n", u.st_data);
    strcat(st_buffer, st_temp);
   }


  //  strcat(st_buffer, "Cookie: PREF=ID=62bf3e453cd19d16:FF=4:LD=en:NR=10:TM=1032994623:LM=1033024920:S=PbEXZv1loxItUXf6; CP=null*");

    //sprintf(st_temp, "Content-Length: %d\r\n", strlen(st_temp)+strlen("Content-Length:"));
  //  strcat(st_buffer, st_temp);


    strcat(st_buffer, "\r\n"); //blank line indicating the content is going to start


    //)
  // log_msg("Grabbing %s.", st_buffer);
  
   
    socket.Write(st_buffer, strlen(st_buffer));
    

    //read what we get
    char st_buff[513];
    memset(st_buff, 0, 513);
    
    int i_read = 0;    
    
    while ( (i_read = socket.Read((char*)&st_buff, 512)) > 0)
    {
        // log_msg(st_buff);
        p_parse->AddText(st_buff, i_read);
    }
    //add ending NULL
    p_parse->AddText("\0", 1);
    return true;
}



