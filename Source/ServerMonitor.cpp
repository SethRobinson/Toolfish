#include "stdafx.h"
#include "ServerMonitor.h"
#include "..\Shared\CTextParse.h"
#include "..\Shared\winsock\winsock_util.h"
#include "..\Shared\winsock\RawSocket.h"
#include "internet_util.h"
#include "spy_util.h"
                         
#include <wininet.h>

bool ProcessMonitor( CEvent *p_event, int i_index)
{
    CAction *p_action = p_event->GetAction(i_index);
   
    TCHAR ws_domain[C_MAX_URL_SIZE];
    TCHAR ws_path[C_MAX_URL_SIZE];
    TCHAR ws_filename[C_MAX_URL_SIZE];
    TCHAR ws_full_url[C_MAX_URL_SIZE];
    TCHAR p_full[C_MAX_URL_SIZE];

    bool b_error = false;
    bool b_https = false; // Will be set by BreakURLDown()
 
    uni uni_filename(p_action->GetFilename());
  
    ReplaceStrings(ws_full_url, uni_filename.GetAuto(), C_MAX_URL_SIZE, wcslen((WCHAR*)uni_filename.us_data), NULL);
     
    //get rid of the starting http if applicapable

    _tcscpy(p_full, ws_full_url);

    if (!BreakURLDown(ws_full_url, p_full, ws_domain, ws_path, ws_filename, &b_https))
   {
       LogError(_T("Error understaing URL, maybe you typed it wrong? (%s), "), uni_filename.GetAuto());
       return true;
   }

    app_glo.SetLastURLCompared(p_full);

 
    //*********** actually connect to the server *************

      char st_server[C_MAX_URL_SIZE]; //temp work buffer
    sprintf(st_server, uni(ws_domain).to_st());
    
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
    int i_start = GetTickCount();
    
    CWizReadWriteSocket socket;
    
    if(!socket.Connect(uni(st_server).GetAuto(), p_action->GetPort()))
    {
        if (!p_action->GetMonitorTimeout())
        {
            //they instructed us to not count connect failure.  maybe they just aren't connected
            //to the net.. oh well, ignore this.
        

            log_msg("Unable to reach %s.  Monitor settings indicate we should not care.", uni(p_action->GetFilename()).to_st());
            return false;
        }

        
        b_error = true; //signal error but don't handle it here
        log_msg("SITE MONITOR FAILED: Website/port %s is unreachable.", uni(p_action->GetFilename()).to_st());
        //let it compute the fail stats later
    }
   
    int i_final = GetTickCount() - i_start;

    char st_result[256];
    strcpy(st_result, "");

    if (!b_error)
    {
        
        
        if (p_action->GetWebPage())
        {
            // Use WinINet to fetch the webpage - supports both HTTP and HTTPS
            
            // Build the full URL
            char st_url[C_MAX_URL_SIZE];
            int i_port = p_action->GetPort();
            
            // Determine if we need to specify a non-default port in the URL
            bool b_default_port = (b_https && i_port == 443) || (!b_https && i_port == 80);
            
            if (b_default_port)
            {
                sprintf(st_url, "%s://%s/%s", b_https ? "https" : "http", st_server, uni(ws_path).to_st());
            }
            else
            {
                sprintf(st_url, "%s://%s:%d/%s", b_https ? "https" : "http", st_server, i_port, uni(ws_path).to_st());
            }
            
            // Open internet session
            HINTERNET hInternet = InternetOpenA("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0;)", 
                INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
            
            if (hInternet)
            {
                // Open URL - handles HTTPS and redirects automatically
                DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;
                if (b_https)
                {
                    dwFlags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
                }
                
                HINTERNET hUrl = InternetOpenUrlA(hInternet, st_url, NULL, 0, dwFlags, 0);
                
                if (hUrl)
                {
                    // Query HTTP status code
                    DWORD dwStatusCode = 0;
                    DWORD dwStatusSize = sizeof(dwStatusCode);
                    HttpQueryInfoA(hUrl, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatusCode, &dwStatusSize, NULL);
                    
                    int i_result_code = (int)dwStatusCode;
                    
                    // Get status text for logging
                    char st_status_text[256] = "";
                    DWORD dwTextSize = sizeof(st_status_text);
                    HttpQueryInfoA(hUrl, HTTP_QUERY_STATUS_TEXT, st_status_text, &dwTextSize, NULL);
                    sprintf(st_result, " (HTTP/%d %s)", i_result_code, st_status_text);
                    
                    // Read response body for size check
                    CTextParse parse;
                    char st_buff[513];
                    DWORD dwBytesRead = 0;
                    
                    while (InternetReadFile(hUrl, st_buff, 512, &dwBytesRead) && dwBytesRead > 0)
                    {
                        parse.AddText(st_buff, dwBytesRead);
                        dwBytesRead = 0;
                    }
                    parse.AddText("\0", 1);
                    
                    bool b_ignore_404 = false;

                    if (_tcsstr(ws_path, _T(".")) == NULL)
                    {
                        //there is no period in this, so let's ignore 404 errors as this is normal when you don't specify
                        //a page
                       b_ignore_404 = true;
                    }
                    
                    if (p_action->GetMonitorError())
                    {
                        if ( 
                          (  (i_result_code >= 400) && (i_result_code <= 415) )
                            ||  (  (i_result_code >= 500) && (i_result_code <= 505) )
                            )
                        {
                            //bad code detected.
                            
                            if ( !((i_result_code == 404) && b_ignore_404) )
                            {
                                log_msg("SITE MONITOR FAILED: Website %s returned error code %d. (%s)", uni(p_action->GetFilename()).to_st(),
                                    i_result_code, st_status_text);
                                b_error = true;
                            }
                        }
                    }
               
                    if (!b_error && (p_action->GetMonitorSize()) )
                    {
                        //let's also check size
                        if (int(parse.GetBuffByteSize()) < p_action->GetMonitorSizeBytes())
                        {
                            //it's too small.
                            log_msg("SITE MONITOR FAILED: Website %s is less than %d bytes. (%s)", 
                                uni(p_action->GetFilename()).to_st(),  p_action->GetMonitorSizeBytes(),
                                st_status_text);
                            b_error = true;
                        }
                    }
                    
                    InternetCloseHandle(hUrl);
                }
                else
                {
                    // Failed to open URL
                    log_msg("SITE MONITOR FAILED: Could not fetch webpage %s (WinINet error %d)", uni(p_action->GetFilename()).to_st(), GetLastError());
                    b_error = true;
                }
                
                InternetCloseHandle(hInternet);
            }
            else
            {
                log_msg("SITE MONITOR FAILED: Could not initialize internet session");
                b_error = true;
            }
        }
        
    }
   
    if (b_error)
    {
        p_action->SetFail(); //add data for statistics
        return true;
    } else
    {
        p_action->SetSuccess(); //add data for statistics
        p_action->AddPing(i_final); //add data for statistics
        
        if (!p_event->GetQuietMode())
        {
            
            char st_temp[512];
            sprintf(st_temp, "Site Monitor: %s reached in %dms.  (Avg ping: %d, uptime is %.0f%%%%)", uni(p_action->GetFilename()).to_st(),
                i_final, p_action->GetAveragePing(), p_action->GetUptime()*100);
            log_msg(st_temp);
        }
    }
    
     return false; //don't trigger, nothing is wrong
}
