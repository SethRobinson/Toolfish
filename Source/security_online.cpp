#include "stdafx.h"
#include "security_online.h"
#include "..\Shared\CTextParse.h"
#include "..\Shared\CProtection.h"


CServerInfo::CServerInfo()
{
    memset(this, 0, sizeof(CServerInfo));

    WORD wVer = MAKEWORD(2, 2);
    WSADATA data;
    //wsadata = new WSADATA;
    if (WSAStartup(wVer, &data) != 0)
    {
        log_error("Unable to init winsock.")        ;
    }   else
    {
        m_b_ready = true;
    }
   
}

CServerInfo::~CServerInfo()
{
    WSACleanup(); 

}

bool CServerInfo::GetAuthorization(char *p_st_code, char *p_st_key_out, char *p_message_out, bool b_return_good_on_bad_code)
{
     char st_server[256]; //temp work buffer
    sprintf(st_server, this->m_st_url);


    sockaddr_in blah;
    hostent *he;
    memset ((char *) &blah,'0', sizeof(blah));
    
    if ((he = gethostbyname(st_server)) != NULL)
    {
        //change it to numbers
        
        memcpy((char *) &blah.sin_addr, he->h_addr, he->h_length );
        sprintf(st_server, "%s", inet_ntoa(blah.sin_addr));
        
   //     log_msg("Converted to %s.", st_server);		
    }
    	CWizReadWriteSocket socket;

		if(!socket.Connect(uni(st_server).GetAuto(), m_i_port))
        {
            strcpy(p_message_out, "Unable to connect with server.  It may be down or rebooting, please try again later.  Sorry for the hassle!");
            return false;
        }

   //     log_msg("Connected to server.");
        
        //let's send over our info and wait the reply
   char st_temp[255];
        sprintf(st_temp, "command=11&request=%s&ver=%.2f&time=%dTOOL283F&&&",p_st_code,
            app_glo.GetVersion(), cls_pro.get_serial() );
        socket.Write(st_temp, strlen(st_temp));

		if (socket.GetStatus() != CWizSyncSocket::STATUS_OK)
		{
			return false;
		}
        //read what we get
        char st_buff[513];
        memset(st_buff, 0, 513);
    
        CTextParse parse;
        int i_read = 0;    
        
        while ( (i_read = socket.Read((char*)&st_buff, 512)) > 0)
        {
      //      log_msg(st_buff);
            parse.AddText(st_buff, i_read);
        }
        //add ending NULL
        parse.AddText("\0", 1);

		if (socket.GetStatus() != CWizSyncSocket::STATUS_OK)
		{
			return false;
		}
        //ok, we should now be holding the reg code.  But what do we do with it
       char *p_line;
  char st_key[2000];
   char st_code[100];

      char st_message[255];

      st_code[0] = 0;

      while (parse.get_next_line(&p_line))
  {
      //log_msg("Line: %s", p_line);

    strcpy(st_key,parse.get_word(1, '|'));
  
      if (stricmp(st_key, "update") == 0)
      {
         strcpy(st_code, parse.get_word(2, '|'));
      }
      if (stricmp(st_key, "message") == 0)
      {
         strcpy(st_message, parse.get_word(2, '|'));
      }

  }
     if (!b_return_good_on_bad_code)
     {
     if (strlen(st_code) < 3)
     {
         //some kind of error
         replace("^", "\n", st_message);
         strcpy(p_message_out, st_message);
         return false;
     }
     }
     //assume the code is good.  won't actually get checked until later
     strcpy(p_st_key_out, st_code);
     return true;
}

bool CServerInfo::GetServerInfo(char p_st_server[], char p_st_file[])
{
    if (!m_b_ready)
    {
        log_error("Unable to init winsock.");
        return false;
    }

    // Build the full URL
    char st_url[512];
    sprintf(st_url, "https://%s/%s", p_st_server, p_st_file);

    // Use WinINet which handles HTTPS automatically
    HINTERNET hInternet = InternetOpenA("Toolfish", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet)
    {
        return false;
    }

    HINTERNET hUrl = InternetOpenUrlA(hInternet, st_url, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hUrl)
    {
        InternetCloseHandle(hInternet);
        return false;
    }

    // Read the response
    CTextParse parse;
    char st_buff[513];
    DWORD dwBytesRead = 0;

    while (InternetReadFile(hUrl, st_buff, 512, &dwBytesRead) && dwBytesRead > 0)
    {
        parse.AddText(st_buff, dwBytesRead);
    }
    parse.AddText("\0", 1);

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);

    // Parse the response
    char *p_line;
    char st_key[2000];

    while (parse.get_next_line(&p_line))
    {
        strcpy(st_key, parse.get_word(1, '|'));

        if (stricmp(st_key, "server_name") == 0)
        {
            strcpy(m_st_url, parse.get_word(2, '|'));
        }
        if (stricmp(st_key, "upgrade_message") == 0)
        {
            strcpy(m_st_upgrade_text, parse.get_word(2, '|'));
        }
        if (stricmp(st_key, "upgrade_link") == 0)
        {
            strcpy(m_st_upgrade_url, parse.get_word(2, '|'));
        }
        if (stricmp(st_key, "version_latest") == 0)
        {
            m_f_version_latest = float(atof(parse.get_word(2, '|')));
        }
        if (stricmp(st_key, "version_required") == 0)
        {
            m_f_version_required = float(atof(parse.get_word(2, '|')));
        }
        if (stricmp(st_key, "server_port") == 0)
        {
            m_i_port = atoi(parse.get_word(2, '|'));
        }
    }

    if (m_i_port == 0)
    {
        return false;
    }

    return true;
}



 UINT CheckForUpdate(LPVOID pVoid)
 {
   app_glo.IncActiveThreadCount();


 //start check procedure
            //   SetDlgItemText(hWnd, IDC_STATUS, "Finding update server...");
                
                if (!server_info.GetServerInfo("www.rtsoft.com", "toolfish/server.txt"))
                {
                    //unable to connect, let them play anyway.
                    goto done;
                }
           //     SetDlgItemText(hWnd, IDC_STATUS, "Found server.  Checking for updates...");
                if (server_info.m_f_version_required > app_glo.GetVersion())
                {
                    int i_ret = MessageBox(app_glo.GetDialogHWND(),uni(server_info.m_st_upgrade_text).GetAuto(), _T("New version released!"), MB_OKCANCEL);
                    if (i_ret == IDOK)
                    {
                        // Use ShellExecute directly on the URL - Windows will route to the default browser
                        // Prepend http:// since the URL may not include the protocol
                        char st_full_url[512];
                        sprintf(st_full_url, "http://%s", server_info.m_st_upgrade_url);
                        int result = (int)ShellExecute(NULL, _T("open"), uni(st_full_url).GetAuto(), NULL, NULL, SW_SHOWNORMAL);
                        ProcessError(app_glo.GetDialogHWND(), result, uni(st_full_url).GetAuto(), uni(server_info.m_st_upgrade_url).GetAuto()); 
                    }
                    
                    
                    PostMessage(app_glo.GetDialogHWND(), WM_QUIT, 0, 0);
                  //  log_msg("Posted quit message.");
                    //in either case, quit out.
                     goto done;
                

      }
                
                if (server_info.m_f_version_latest > app_glo.GetVersion())
                {
                    // Convert ^ to newlines in the message (same convention as GetAuthorization)
                    replace("^", "\n", server_info.m_st_upgrade_text);
                    LogMsg("Update available: %s", server_info.m_st_upgrade_text);
                    LogMsg("Download: %s", server_info.m_st_upgrade_url);
                }
                else
                {
                    LogMsg("Checked for updates: V%.2f is the latest version.", server_info.m_f_version_latest);
                }
                
done:
    app_glo.DecActiveThreadCount();


   return 0;

 }
