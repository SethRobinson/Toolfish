#include "CAction.h"

CActionInfo g_actions; //general action information
TCHAR g_action_buf[C_MAX_URL_SIZE];



WCHAR * CAction::GetBufferPointer()
{
    return m_p_buffer;
} 


TCHAR * CAction::GetName(bool b_long)
{
    TCHAR ws_temp[C_MAX_URL_SIZE];
   
    unsigned int i_truncate_size = 60;

    if (b_long) i_truncate_size = 127; //it can be bigger

    if (m_i_action_type == -1)
    return _T("Disabled");
  if (GetActionType() == C_ACTION_COMPARE)
    {
#ifdef _UNICODE
      _tcscpy(ws_temp, m_filename);
#else
      _tcscpy(ws_temp, uni(m_filename).to_st());
      
#endif
      
      if (_tcslen(ws_temp) > i_truncate_size)
       {
         //it's kind of long, lets' truncate it and shot dots
         ws_temp[i_truncate_size ] = '.';
         ws_temp[i_truncate_size +1] = '.';
         ws_temp[i_truncate_size +2] = '.';
         ws_temp[i_truncate_size +3] = 0;

       }
           _stprintf(g_action_buf, _T("Web Compare of \"%s\""), ws_temp);
           if (b_long)

           {
               //less info is ok
               _stprintf(g_action_buf, _T("URL: \"%s\""), ws_temp);

           }
           
           return ((TCHAR*)&g_action_buf);
    }

    if (GetActionType() == C_ACTION_WEBSITE)
    {
#ifdef _UNICODE
        _tcscpy(ws_temp, m_filename);
#else
        _tcscpy(ws_temp, uni(m_filename).to_st());
        
#endif
       if (_tcslen(ws_temp) > i_truncate_size)
       {
         //it's kind of long, lets' truncate it and shot dots
           ws_temp[i_truncate_size ] = '.';
         ws_temp[i_truncate_size +1] = '.';
         ws_temp[i_truncate_size +2] = '.';
         ws_temp[i_truncate_size +3] = 0;


       }
           _stprintf(g_action_buf, _T("Open \"%s\""), ws_temp);
        return ((TCHAR*)&g_action_buf);
    }

      if (GetActionType() == C_ACTION_MESSAGE)
    {
      
        TCHAR ws_say[12] = _T("");  
        TCHAR ws_log[12] = _T("");  
        TCHAR ws_popup[12] = _T("");  
        TCHAR ws_macro[12] = _T("");  
          
        if (m_b_log) _tcscpy(ws_log, _T("(log)"));
        if (m_b_popup) _tcscpy(ws_popup, _T("(popup)"));
        if (m_b_say) _tcscpy(ws_macro, _T("(say)"));
        if (m_b_macro) _tcscpy(ws_macro, _T("(macro)"));
        if (m_b_email) _tcscpy(ws_macro, _T("(email)"));
#ifdef _UNICODE
        _tcsncpy(ws_temp, this->GetBufferPointer(), i_truncate_size);
#else
        _tcsncpy(ws_temp, uni(this->GetBufferPointer()).to_st(), i_truncate_size);

#endif
        ws_temp[256] = NULL; //add a null just in case the buffer was larger than 50
        if (_tcslen(ws_temp) > i_truncate_size)
       {
         //it's kind of long, lets' truncate it and shot dots
              ws_temp[i_truncate_size ] = '.';
         ws_temp[i_truncate_size +1] = '.';
         ws_temp[i_truncate_size +2] = '.';
         ws_temp[i_truncate_size +3] = 0;


       }
           _stprintf(g_action_buf, _T("%s%s%s%s \"%s\""),ws_say, ws_log, ws_popup, ws_macro, ws_temp);
        return ((TCHAR*)&g_action_buf);
    }
   
      

       if (GetActionType() == C_ACTION_SOUND)
    {
#ifdef _UNICODE
           _tcscpy(ws_temp, m_filename);
#else
           _tcscpy(ws_temp, uni(m_filename).to_st());
#endif
           if (_tcslen(ws_temp) > i_truncate_size)
       {
         //it's kind of long, lets' truncate it and shot dots
            ws_temp[i_truncate_size ] = '.';
         ws_temp[i_truncate_size +1] = '.';
         ws_temp[i_truncate_size +2] = '.';
         ws_temp[i_truncate_size +3] = 0;


       }
           _stprintf(g_action_buf, _T("Play \"%s\""), ws_temp);
        return ((TCHAR*)&g_action_buf);
    }

      

        if (GetActionType() == C_ACTION_RUN)
    {
#ifdef _UNICODE
            _tcscpy(ws_temp, m_filename);
#else
            _tcscpy(ws_temp, uni(m_filename).to_st());

#endif
       if (_tcslen(ws_temp) > i_truncate_size)
       {
         //it's kind of long, lets' truncate it and shot dots
            ws_temp[i_truncate_size ] = '.';
         ws_temp[i_truncate_size +1] = '.';
         ws_temp[i_truncate_size +2] = '.';
         ws_temp[i_truncate_size +3] = 0;


       }
           _stprintf(g_action_buf, _T("Open \"%s\""), ws_temp);
        return ((TCHAR*)&g_action_buf);
    }

        if (GetActionType() == C_ACTION_EMAIL)
        {
            
#ifdef _UNICODE
            _stprintf(g_action_buf, _T("Check Email (%s on %s)"), this->GetLogon(),
                this->GetEmailServer());
#else
            _stprintf(g_action_buf, _T("Check Email (%s on %s)"), uni(GetLogon()).to_st(),
                uni(this->GetEmailServer()).to_st());
#endif
            //check for any length problems
            
            if (_tcslen(g_action_buf) > i_truncate_size)
            {
                //it's kind of long, lets' truncate it and shot dots
                g_action_buf[i_truncate_size ] = '.';
                g_action_buf[i_truncate_size +1] = '.';
                g_action_buf[i_truncate_size +2] = '.';
                g_action_buf[i_truncate_size +3] = 0;
            }
            return ((TCHAR*)&g_action_buf);
            
        }

       if (GetActionType() == C_ACTION_TTS)
        {
         if (GetTTS())
         {
         _stprintf(g_action_buf, _T("Enable Toolfish's Text To Speech"));
          }   else
         {
          _stprintf(g_action_buf, _T("Disable Toolfish's Text To Speech"));
        
         }
        return ((TCHAR*)&g_action_buf);
       } 

     


        if (GetActionType() == C_ACTION_SEND)
        {
        
#ifdef _UNICODE
            _stprintf(g_action_buf, _T("Send Command To App (%s)"),
              GetBufferPointer());
#else
            _stprintf(g_action_buf, _T("Send Command To App (%s)"),
              uni(GetBufferPointer()).st_data);
            
#endif
            return ((TCHAR*)&g_action_buf);
        }
        
        if (GetActionType() == C_ACTION_DELAY)
        {
            
            _stprintf(g_action_buf, _T("Delay (%d ms)"), GetDelay());
            
            return ((TCHAR*)&g_action_buf);
        }
        if (GetActionType() == C_ACTION_LEET)
        {
            
            _stprintf(g_action_buf, _T("Leet-Type Toggle"));
            
            return ((TCHAR*)&g_action_buf);
        }
        

    

        if (GetActionType() == C_ACTION_REBOOT)
        {
            if (GetRebootType() == C_ACTION_REBOOT_LOGOFF)
            {
                _stprintf(g_action_buf, _T("Logoff User"));
            }            

                  if (GetRebootType() == C_ACTION_REBOOT_SHUTDOWN)
            {
                _stprintf(g_action_buf, _T("Shutdown Computer"));
            }            

                  if (GetRebootType() == C_ACTION_REBOOT_RESTART)
            {
                _stprintf(g_action_buf, _T("Reboot Computer"));
            }            
            
            return ((TCHAR*)&g_action_buf);
        }
        


       if (GetActionType() == C_ACTION_SMARTMUTE)
        {
         if (GetTTS())
         {
         _stprintf(g_action_buf, _T("Enable Smart Mute"));
          }   else
         {
          _stprintf(g_action_buf, _T("Disable Smart Mute"));
        
         }
        return ((TCHAR*)&g_action_buf);
       }
       if (GetActionType() == C_ACTION_VOLUME)
       {
           _stprintf(g_action_buf, _T("Set System Volume to %d%%"), GetVolume()*10);
           return ((TCHAR*)&g_action_buf);
       }
	   if (GetActionType() == C_ACTION_MIC_VOLUME)
	   {
		   _stprintf(g_action_buf, _T("Set Mic Volume to %d%%"), GetVolume());
		   return ((TCHAR*)&g_action_buf);
	   }
       if (GetActionType() == C_ACTION_MONITOR)
       {
#ifdef _UNICODE
           _stprintf(g_action_buf, _T("Site Monitor of %s (port %d)"),
               this->GetFilename(), this->GetPort());
#else
           _stprintf(g_action_buf, _T("Site Monitor of %s (port %d)"),
               uni(this->GetFilename()).to_st(), this->GetPort());

#endif
         
               if (_tcslen(g_action_buf) > i_truncate_size)
            {
                //it's kind of long, lets' truncate it and shot dots
                g_action_buf[i_truncate_size ] = '.';
                g_action_buf[i_truncate_size +1] = '.';
                g_action_buf[i_truncate_size +2] = '.';
                g_action_buf[i_truncate_size +3] = 0;
            }
       
           
           return ((TCHAR*)&g_action_buf);
       }
       
    return (TCHAR*)&g_actions.m_action[m_i_action_type].st_name;
}



CAction::CAction()
    {
        memset(this, 0, sizeof(CAction));
        m_i_action_type = -1;
        m_b_content = true;
        m_f_content_percent = 0.05f;
    }

CAction::~CAction()
{
    //if we had a buffer allocated, better get rid of it
    SAFE_DELETE_ARRAY(m_p_buffer);

}

void CAction::SetFilename( const WCHAR *p_new)
{
    wcscpy(m_filename, p_new);
}


CDialog* CAction::InitActionDialog()
{
    return g_actions.CreateActionDialog(m_i_action_type, this);
}

void CAction::CopyToBuffer(const WCHAR * p_wchar)
{
    //if we had something in here, get rid of it
    SAFE_DELETE_ARRAY(m_p_buffer);

    //get the length of what we're gonna copy
    int i_size = wcslen(p_wchar);
    m_p_buffer= (WCHAR*) new WCHAR[i_size+1]; //1 extra for the null
    wcscpy(m_p_buffer, p_wchar);
}

bool CAction::Save(FILE * fp)
{
   //first save size
   	int i_size = sizeof(CAction);
  
    //first write the size of this struct
  	fwrite(&i_size, sizeof(int), 1, fp);

    //now write the trigger
      if (fwrite(this, i_size, 1, fp) <= 0)
	{
	  log_error("Error writing action info");
	}


     //great, but hold everything. if our buffer isn't null we need to save that too
     
      int i_buffer_size_in_tchar = 0;
      if (this->GetBufferPointer())
      {
         i_buffer_size_in_tchar = wcslen(GetBufferPointer())+1; //one extra for the null
      }

      //first write how many chars we got
      if (fwrite(&i_buffer_size_in_tchar, sizeof(int) ,1, fp) <= 0)
      {
          log_error("Error writing tchar size");
      }
      
     if (i_buffer_size_in_tchar > 0)
     {
         if (fwrite(GetBufferPointer(), 2, i_buffer_size_in_tchar, fp) <= 0)
         {
             log_error("Error writing action buffer info");
         }

     }

    return true;
}

bool CAction::Load(FILE * fp)
{
    //clear memory
    SAFE_DELETE_ARRAY(m_p_buffer);
  
    //first load size
   	int i_size = 0;
  
    //first read the size of this struct
  	fread(&i_size, sizeof(int), 1, fp);
	m_p_buffer = NULL;

    //now write the trigger
      if (fread(this, i_size, 1, fp) <= 0)
	{
		m_p_buffer = NULL;

		log_error("Error reading action info");
      return false;
	}

#ifdef _DEBUG
	  if (i_size != sizeof(*this))
	  {
		  LogMsg(_T("Action Size difference detected, it is %d but we are %d"),
			  i_size, sizeof(*this));
	  }
#endif
    //clear bad pointers
      m_p_buffer = NULL;

      //we may need to load buffer stuff
     int i_tchar_count_with_null = 0;
     fread(&i_tchar_count_with_null, sizeof(int), 1, fp);


      if (i_tchar_count_with_null > 0)
      {
          //init memory for it
          m_p_buffer= (WCHAR*) new WCHAR[i_tchar_count_with_null];
          //we need to actually load it in
         if (fread(m_p_buffer, 2, i_tchar_count_with_null, fp) <= 0)
         {
             log_error("Error loading action text buffer info");
         }

      }

    return true;
}


void CAction::ClearMonitorStats()
{

    m_i_min_ping = C_ACTION_NO_PING_DATA;
    m_i_max_ping = C_ACTION_NO_PING_DATA;

    m_i_total_ping = 0;
    m_i_ping_count = 0;

    m_i_monitor_fail = 0;
    m_i_monitor_success = 0;



}

void CAction::AddPing(int i_ms)
{
    m_i_total_ping += i_ms;
    m_i_ping_count++;

    if (m_i_max_ping == C_ACTION_NO_PING_DATA) m_i_max_ping = i_ms;
    if (m_i_min_ping == C_ACTION_NO_PING_DATA) m_i_min_ping = i_ms;

    if (i_ms > m_i_max_ping) m_i_max_ping = i_ms;
    if (i_ms < m_i_min_ping) m_i_min_ping = i_ms;

}


int CAction::GetAveragePing()
{
    if (m_i_ping_count == 0) return 0; //no data yet
    return int( float(m_i_total_ping)/ float(m_i_ping_count));
}

float CAction::GetUptime()
{
    if (m_i_monitor_fail == 0 && m_i_monitor_success == 0) return 1.0f; //no data yet, so 100 percent uptime
    return (  float(m_i_monitor_success) / float (GetMonitorSamples()));
}


