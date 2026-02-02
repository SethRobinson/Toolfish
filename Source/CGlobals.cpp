#include "stdafx.h"
#include "CGlobals.h"
#include "CEventList.h"
#include "..\Shared\CSpeech.h"  
#include "CDlgMute.h"      
#include "..\Shared\winsock\winsock_util.h"                             
#include "demo_util.h"
#include "..\Shared\CProtection.h"
#include "CTextParse.h"

CApplicationGlobals app_glo;

CConfig glo;


CServerInfo server_info; //some copy protection stuff

CApplicationGlobals::CApplicationGlobals()
{
    memset(this, 0, sizeof(CApplicationGlobals));
    _tcscpy(m_st_program_name, _T("Toolfish"));
    m_f_version = C_F_VERSION;
    m_p_events = new CEventList();
    _tcscpy(m_ws_last_url_compared, _T("(none)"));

    _tcscpy(m_st_home_dir, GetExeDir());

    //create a mutex that we'll need
    m_thread_mutex =  CreateMutex(NULL, FALSE,_T("ToolfishThread"));  // name of mutex
    
    if (m_thread_mutex == NULL) 
    {
        LogError(_T("Unable to create mutex.  Threads may not be safe."));
        // Check for error.
    }
    m_timer_security_check.set_interval(random_range(45,77)*1000*60);
    m_timer_shut_down_message.set_interval(4000);
}



CApplicationGlobals::~CApplicationGlobals()
{
    if (m_b_muted)
    {
        //unmute the computer on exit
        DeactivateMute();
    }
    
    //I hope we already saved this...
    SAFE_DELETE(m_p_events);
    SAFE_DELETE(m_p_speech); 

  
}


void CApplicationGlobals::SecurityCheck()
{
    if (m_timer_security_check.interval_reached())
    {
        if (GetTrialDaysLeft() > -1) 
       {
       
            //trial mode.
             InitTrialStuff();
        
       
           if (GetTrialDaysLeft() == 0)
           {
               //zero means out of days, this can never happen.
               m_b_tampered = true;
               m_b_leave_message = true;
           }
        } else
        {
            //they claim they are using the registered version... let's just
            //see about that.
            
          cls_pro.demake(glo.m_st_code, glo.m_st_unlock_key);
         
         
         if (!cls_pro.is_validated())
         {
             //code was bad.  let's do an easy to hack message.  (don't worry, the tricky security comes later)
             //LogMsg("Error with unlock key %s, code %s", glo.m_st_code, glo.m_st_unlock_key);
             glo.m_st_code[0] = 0;
             glo.m_st_unlock_key[0] = 0;
             FileConfigSave(&glo);
             m_b_tampered = true;
         }


        }
    }
}

void CApplicationGlobals::ShutDownMessageCheck()
{
    if (m_timer_shut_down_message.interval_reached())
    {
       if (m_b_leave_message)
       {
           MessageBox(NULL, _T("No valid unlock key or trial license found.  Program will shut down now."), _T("Error"), MB_ICONSTOP |MB_OK);
           PostQuitMessage(0);
       }
    }
}


int CApplicationGlobals::GetActiveThreadCount()
{
   DWORD dwWaitResult = WaitForSingleObject(m_thread_mutex, 5000L); // five-second time-out interval
   if (dwWaitResult != WAIT_OBJECT_0)
   {
       LogError(_T("Error getting access to a mutex.  Non fatal."));
   }
 
   int i_temp = m_g_threads_active;


   if (! ReleaseMutex(m_thread_mutex))
   { 
         LogError(_T("Error releasing mutex.  Non fatal."));
 
   }
  return i_temp;
}


void CApplicationGlobals::IncActiveThreadCount()
{
   DWORD dwWaitResult = WaitForSingleObject(m_thread_mutex, 5000L); // five-second time-out interval
   if (dwWaitResult != WAIT_OBJECT_0)
   {
       LogError(_T("Error getting access to a mutex.  Non fatal."));
   }
 
   m_g_threads_active++;


   if (! ReleaseMutex(m_thread_mutex))
   { 
         LogError(_T("Error releasing mutex.  Non fatal."));
 
   }
  
}

void CApplicationGlobals::DecActiveThreadCount()
{
   DWORD dwWaitResult = WaitForSingleObject(m_thread_mutex, 5000L); // five-second time-out interval
   if (dwWaitResult != WAIT_OBJECT_0)
   {
       LogError(_T("Error getting access to a mutex.  Non fatal."));
   }
 
   m_g_threads_active--;

   if (m_g_threads_active < 0)
   {
       LogError(_T("How could %d threads be active?"), m_g_threads_active);
   }

   if (! ReleaseMutex(m_thread_mutex))
   { 
         LogError(_T("Error releasing mutex.  Non fatal."));
 
   }
  
}



bool CApplicationGlobals::TimeToMute()
{
  	if ( (m_timer_mute.GetTimerStart()+m_timer_mute.GetInterval()) <= (int)GetTickCount())
  {
        //same as interval_reached but we don't want to reset the timer
        return true;
  }
 return false;
}

void CApplicationGlobals::CleanUpMouseMovement()
{
    if (m_i_mouse_counter > 0)
    m_i_mouse_counter--;
}

bool CApplicationGlobals::MouseMoved()
{
    m_i_mouse_counter++;
    if (m_i_mouse_counter > C_MOUSE_SENSITIVITY) return true;

    return false;
}

void CApplicationGlobals::ResetMuteTimer()
{
//    log_msg("Reset mute timer.");
    int i_temp = glo.m_i_mute_time * 1000; //convert to seconds
    if (glo.m_i_mute_time_type == C_TIMER_TYPE_MINUTES)
    {
        //minutes, not seconds
        i_temp *= 60;
    }
    m_timer_mute.set_interval(i_temp);
   

}                             


//if this isn't called you're gonna have probs dude
void CApplicationGlobals::InitSpeech()
{
   SAFE_DELETE(m_p_speech); //just in case we called it more than once on accident
   m_p_speech = new CSpeech();
   glo.m_b_disable_tts = false; //it's enabled now
   m_p_speech->say(""); //Vista needs this to init speech for some reason

}

void CApplicationGlobals::ActivateSpeech(bool b_turn_on)
{
    //flip the speech system on or off
    if (!b_turn_on)
    {
        //delete if it was active
        SAFE_DELETE(m_p_speech);
        glo.m_b_disable_tts = true; //it's disabled now
        return;
    }
    
    if (m_p_speech) return; //it was already initted
    InitSpeech();

}


UINT GetIPAddressFromServer(LPVOID pVoid)
{
    app_glo.IncActiveThreadCount();

    CTextParse parse;
    struct sockaddr_in blah;
    struct hostent *he;
    memset ((char *) &blah,'0', sizeof(blah));
    char st_server[MAX_PATH];
    strcpy(st_server, "rtsoft.com");

    if ((he = gethostbyname(st_server)) != NULL)
    {
        //change it to numbers
        memcpy((char *) &blah.sin_addr, he->h_addr, he->h_length );
        sprintf(st_server, "%s", inet_ntoa(blah.sin_addr));
        
     //   	LogMsg(_T("Converted to %s."), uni(st_server).GetAuto());		
    }
    
    
    CWizReadWriteSocket socket;
 
    if(!socket.Connect(uni(st_server).GetAuto(), 80))
       {
            LogMsg(_T("Failed to get IP address, rtsoft.com might be down."));
            goto done;
       }
    char st_temp[255];

      sprintf(st_temp, "GET /htbin/ip.php HTTP/1.1\r\nAccept: */*\r\nHost: %s\r\nUser-Agent: Toolfish\r\n\r\n", "rtsoft.com");
      socket.Write(st_temp, strlen(st_temp));
        //read what we get
        char st_buff[MAX_PATH];
        memset(st_buff, 0, MAX_PATH);
        int i_read;
        i_read = 0;    
        
        while ( (i_read = socket.Read((char*)&st_buff, MAX_PATH-1)) > 0)
        {
             parse.AddText(st_buff, i_read);
        }
  
        //add ending NULL
        parse.AddText("\0", 1);
  
  char *p_line;
  bool bIPWasFound = false;

  char st_key[6000];  //why so big?  to not crash if our website got screwed and gave huge not found messages
  while (parse.get_next_line(&p_line))
  {
    
      strcpy(st_key,parse.get_word(1, '|'));
      if (stricmp(st_key, "IPADDRESS") == 0)
      {
          app_glo.SetIP(parse.get_word(2, '|'));
          bIPWasFound = true;
      }
      
  }

    socket.Close();
  
    if (!bIPWasFound)
    {
        LogMsg("Unable to find latest version from server");
    }
    else
    {
#ifdef _DEBUG
        LogMsg("Detected IP address.");
#endif
    }


done:

        app_glo.DecActiveThreadCount();
        return 0;
}

void CApplicationGlobals::RetrieveIPAddress()
{
   if (glo.m_b_server_ip_disabled)
   {
    //simple way
    GetLocalAddress((char*)&m_st_ip);
   } else
   {
    //complex way
    AfxBeginThread(GetIPAddressFromServer, 0);
   }
}

char * CApplicationGlobals::GetIP()
{ 
    //might as well check the IP thingie again in case it's changed
    //if (using server side ip checking)
    if (!glo.m_b_server_ip_disabled)
    {

    } else
    {
        RetrieveIPAddress();
    }
    return m_st_ip;
};


void log_msg(const LPCSTR lpFormat, ...)
{
     
     va_list Marker;
	char szBuf[2048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
    strcat(szBuf, "\r\n");

#ifdef _DEBUG
    OutputDebugStringA(szBuf);
#endif    

     if (app_glo.GetDialogHWND())
     {
         //this should never happen
    
    //Let's send it via a post to the window, safer this way.
	char *st_buffer;
	st_buffer = (char *) calloc(sizeof(szBuf)+1,1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	strcpy(st_buffer, szBuf);
	PostMessage(app_glo.GetDialogHWND(),WM_ADD_TO_LOG, 0, (LONG) st_buffer);
     } 
	
}


void LogMsg(const TCHAR * lpFormat, ...)
{
     
     va_list Marker;
	TCHAR szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	_vstprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
    _tcscat(szBuf, _T("\r\n"));
#ifdef _DEBUG
    OutputDebugString(szBuf);
#endif
	
     if (app_glo.GetDialogHWND())
     {
         //this should never happen
    
    //Let's send it via a post to the window, safer this way.
	TCHAR *st_buffer;
	st_buffer = (TCHAR *) calloc( (sizeof(szBuf)+1)*sizeof(TCHAR),1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	_tcscpy(st_buffer, szBuf);
#ifdef _UNICODE
    PostMessage(app_glo.GetDialogHWND(),WM_ADD_TO_LOGW, 0, (LONG) st_buffer);
#else
    PostMessage(app_glo.GetDialogHWND(),WM_ADD_TO_LOG, 0, (LONG) st_buffer);

#endif
     } 
	
}


void LogMsg(char * lpFormat, ...)
{
	char szBuf[4048];

	va_list Marker;

	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	LogMsg(uni(szBuf).GetAuto());

}

void LogError(const TCHAR * lpFormat, ...)
{
     
     va_list Marker;
	TCHAR szBuf[2048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	_vstprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
    _tcscat(szBuf, _T("\r\n"));

#ifdef _DEBUG
   
    
    OutputDebugString(szBuf);
#endif

TCHAR st_save_fn[MAX_PATH];

_stprintf(st_save_fn, _T("%s\\debug.txt"), app_glo.GetHomeDir());

#ifdef _UNICODE
    add_text(uni(szBuf).to_st(), uni(st_save_fn).to_st());
#else
    add_text(szBuf, uni(st_save_fn).to_st());
#endif
     if (app_glo.GetDialogHWND())
     {
         //this should never happen
    
    //Let's send it via a post to the window, safer this way.
	TCHAR *st_buffer;
	st_buffer = (TCHAR *) calloc( (sizeof(szBuf)+1)*sizeof(TCHAR),1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	_tcscpy(st_buffer, szBuf);
#ifdef _UNICODE
    PostMessage(app_glo.GetDialogHWND(),WM_ADD_TO_LOGW, 0, (LONG) st_buffer);
#else
    PostMessage(app_glo.GetDialogHWND(),WM_ADD_TO_LOG, 0, (LONG) st_buffer);

#endif
     } 
	
}




void log_error(const LPCSTR lpFormat, ...)
{
     
     va_list Marker;
	char szBuf[2048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
    strcat(szBuf, "\r\n");


   add_text(szBuf, "debug.txt");
#ifdef _DEBUG 
    OutputDebugStringA(szBuf);
#endif
    

     if (app_glo.GetDialogHWND())
     {
         //this should never happen
    
    //Let's send it via a post to the window, safer this way.
	char *st_buffer;
	st_buffer = (char *) calloc(sizeof(szBuf)+1,1); //allocate memory on the heap
	//don't worry, when the post is recieved it will deallocate it
	if (st_buffer)
    {
    
    strcpy(st_buffer, szBuf);
	PostMessage(app_glo.GetDialogHWND(),WM_ADD_TO_LOG, 0, (LONG) st_buffer);
    } else
    {
        //um, we have a major problem...
        assert(0);
    }
     } 
	
}


//code is unused, for the future if I want certain kinds of blinks or whatever  
void SetTrayBlinking(bool b_blink_on, long i_code)
{
    
    if (app_glo.GetDialogHWND())
    {
        if (b_blink_on)
        {
        
        PostMessage(app_glo.GetDialogHWND(),WM_TRAY_BLINK_ON, 0, (LONG) i_code);
        } else
        {
        PostMessage(app_glo.GetDialogHWND(),WM_TRAY_BLINK_OFF, 0, (LONG) i_code);

        }
    } else
    {
        //um, we have a major problem...
        assert(0);
    }
    
    
}

