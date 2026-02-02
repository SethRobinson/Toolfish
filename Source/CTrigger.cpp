#include "stdafx.h"
#include "CTrigger.h"
#include "..\Shared\CProtection.h"
#include "CEvent.h"

CTriggerInfo g_triggers; //general trigger information

TCHAR g_trigger_buf[512];
class CEvent;


CDialog * CTriggerInfo::CreateTriggerDialog(int i_trigger_type, CTrigger *p_trigger)
       {
           CDialog *p_dlg = NULL;

           if (i_trigger_type == C_TRIGGER_TIME)
           {
               p_dlg = new CDlgTriggerTime();
               //set it up 
               ((CDlgTriggerTime*)p_dlg)->SetTriggerPointer(p_trigger);
           } else
           
           if (i_trigger_type == C_TRIGGER_HOTKEY)
           {
               p_dlg = new CDlgTriggerHotkey();
               //set it up 
               ((CDlgTriggerHotkey*)p_dlg)->SetTriggerPointer(p_trigger);
           } else
           if (i_trigger_type == C_TRIGGER_TIME_RANGE)
           {
               p_dlg = new CDlgTimeRange();
               //set it up 
               ((CDlgTimeRange*)p_dlg)->SetTriggerPointer(p_trigger);
              
           
           } else
       if (i_trigger_type == C_TRIGGER_INACTIVITY)
           {
               p_dlg = new CDlgTriggerInactivity();
               //set it up 
               ((CDlgTriggerInactivity*)p_dlg)->SetTriggerPointer(p_trigger);
           } else
       
               if (i_trigger_type == C_TRIGGER_DAY_FILTER)
           {
               p_dlg = new CDlgTriggerDayFilter();
               //set it up 
               ((CDlgTriggerDayFilter*)p_dlg)->SetTriggerPointer(p_trigger);
           } else
         
               if (i_trigger_type == C_TRIGGER_PROCESS)
           {
               p_dlg = new CDlgTriggerProcess();
               //set it up 
               ((CDlgTriggerProcess*)p_dlg)->SetTriggerPointer(p_trigger);
           } else
           {
             log_error("Don't know how to handle this kind of trigger.");
            
           }

           return p_dlg;
       }


  


CTrigger::CTrigger()
    {
        memset(this, 0, sizeof(CTrigger));
        m_i_trigger_type = -1;
        m_time = static_cast<time_t>(CTime::GetCurrentTime().GetTime()); 
        SetRescheduleActive(true);
        SetRescheduleRaw(1);
        SetRescheduleType(C_TRIGGER_TIME_DAYS);
        for (int i=0; i < 7; i++)
        {
            m_b_a_day_active[i] = true; //default to all days
        }
 }




//for use with the Time Range trigger only
bool CTrigger::TimeIsBetweenRange(CTime current_time)
{
    //remove month info
    
    
    if (current_time > m_time_end) return false;
    if (current_time < m_time) return false;
    
    return true;
}



WCHAR * CTrigger::GetBufferPointer()
{
    return m_p_buffer;
} 

void CTrigger::CopyToBuffer(const WCHAR * p_wchar)
{
    //if we had something in here, get rid of it
    SAFE_DELETE_ARRAY(m_p_buffer);

    //get the length of what we're gonna copy
    int i_size = wcslen(p_wchar);
    m_p_buffer= (WCHAR*) new WCHAR[i_size+1]; //1 extra for the null
    wcscpy(m_p_buffer, p_wchar);
}


TCHAR * CTrigger::GetName()
{
    if (m_i_trigger_type == -1)
    return _T("Disabled");

    if (m_i_trigger_type == C_TRIGGER_HOTKEY)
    {
        TCHAR ws_control[12], ws_alt[12], ws_shift[12];
         //show the hotkey type
        if (m_b_ctrl) _tcscpy(ws_control, _T("Ctrl-")); else ws_control[0] = 0;
        if (m_b_alt) _tcscpy(ws_alt, _T("Alt-")); else ws_alt[0] = 0;
        if (m_b_shift) _tcscpy(ws_shift, _T("Shift-")); else ws_shift[0] = 0;
        TCHAR st_key[32];
        _stprintf(st_key, _T("%c"), TCHAR(m_vk_key));

        if (m_vk_key == VK_F1)    _stprintf(st_key, _T("F1"));
        if (m_vk_key == VK_F2)    _stprintf(st_key, _T("F2"));
        if (m_vk_key == VK_F3)    _stprintf(st_key, _T("F3"));
        if (m_vk_key == VK_F4)    _stprintf(st_key, _T("F4"));
        if (m_vk_key == VK_F5)    _stprintf(st_key, _T("F5"));
        if (m_vk_key == VK_F6)    _stprintf(st_key, _T("F6"));
        if (m_vk_key == VK_F7)    _stprintf(st_key, _T("F7"));
        if (m_vk_key == VK_F8)    _stprintf(st_key, _T("F8"));
        if (m_vk_key == VK_F9)    _stprintf(st_key, _T("F9"));
        if (m_vk_key == VK_F10)    _stprintf(st_key, _T("F10"));
        if (m_vk_key == VK_F11)    _stprintf(st_key, _T("F11"));
        if (m_vk_key == VK_F12)    _stprintf(st_key, _T("F12"));
        if (m_vk_key == VK_F13)    _stprintf(st_key, _T("F13"));
        if (m_vk_key == VK_F14)    _stprintf(st_key, _T("F14"));
        if (m_vk_key == VK_F15)    _stprintf(st_key, _T("F15"));
        if (m_vk_key == VK_F16)    _stprintf(st_key, _T("F16"));
        if (m_vk_key == VK_F17)    _stprintf(st_key, _T("F17"));
        if (m_vk_key == VK_F18)    _stprintf(st_key, _T("F18"));
        if (m_vk_key == VK_F19)    _stprintf(st_key, _T("F19"));
        if (m_vk_key == VK_F20)    _stprintf(st_key, _T("F20"));
        if (m_vk_key == VK_F21)    _stprintf(st_key, _T("F21"));
        if (m_vk_key == VK_F22)    _stprintf(st_key, _T("F22"));


        
        
        _stprintf(g_trigger_buf, _T("Hotkey: %s%s%s%s"), ws_control, ws_alt, ws_shift, st_key);
        return ((TCHAR*)&g_trigger_buf);
    }

    if (m_i_trigger_type == C_TRIGGER_TIME)
    {
        TCHAR ws_repeat[30] = _T("");
        
        if (GetRescheduleActive())
        {
            if (GetRescheduleType() ==  C_TRIGGER_TIME_SECONDS)
            {
                _stprintf(ws_repeat, _T(" (every %d secs)"), GetRescheduleRaw());
            }
            if (GetRescheduleType() ==  C_TRIGGER_TIME_MINUTES)
            {
                _stprintf(ws_repeat, _T(" (every %d mins)"), GetRescheduleRaw());
            }
            if (GetRescheduleType() ==  C_TRIGGER_TIME_HOURS)
            {
                if (GetRescheduleRaw() == 1)
                {
                      _stprintf(ws_repeat, _T(" (every %d hour)"), GetRescheduleRaw());
       
                }   else
                {
                  _stprintf(ws_repeat, _T(" (every %d hours)"), GetRescheduleRaw());
                }
            }
       
          if (GetRescheduleType() ==  C_TRIGGER_TIME_DAYS)
            {
                _stprintf(ws_repeat, _T(" (every %d day(s))"), GetRescheduleRaw());
            }
          
        }

        _stprintf(g_trigger_buf, _T("Timer%s"), ws_repeat);
        return ((TCHAR*)&g_trigger_buf);

    }
    if (m_i_trigger_type == C_TRIGGER_TIME_RANGE)
    {
          _stprintf(g_trigger_buf, _T("Time Range"));
        return ((TCHAR*)&g_trigger_buf);
    }

    if (m_i_trigger_type == C_TRIGGER_INACTIVITY)
    {
          _stprintf(g_trigger_buf, _T("Inactivity"));
        return ((TCHAR*)&g_trigger_buf);
    }
     if (m_i_trigger_type == C_TRIGGER_DAY_FILTER)
    {
          _stprintf(g_trigger_buf, _T("Day Filter"));
        return ((TCHAR*)&g_trigger_buf);
    }

      if (m_i_trigger_type == C_TRIGGER_PROCESS)
    {
          TCHAR t_temp[60];

          if (GetProcessFoundField() == C_TRIGGER_PROCESS_EXIST)
          {
              _tcscpy(t_temp, _T("exists"));
          }   else
          if (GetProcessFoundField() == C_TRIGGER_PROCESS_NOT_EXIST)
          {
              _tcscpy(t_temp,_T("doesn't exist"));

          }   else
          {
              _tcscpy(t_temp,_T("ERROR"));
            }
          
#ifdef _UNICODE
          _stprintf(g_trigger_buf, _T("If process \"%s\" %s"),
              GetBufferPointer(), t_temp);
#else
          _stprintf(g_trigger_buf, _T("If process \"%s\" %s"),
              uni(GetBufferPointer()).st_data, t_temp);

#endif
        return ((TCHAR*)&g_trigger_buf);
    }


    //default

    return (TCHAR*)&g_triggers.m_trigger[m_i_trigger_type].st_name;
}

void CTrigger::SetTime(CTime *p_time)
{
	m_time = static_cast<time_t>(p_time->GetTime());
}
long CTrigger::GetRescheduleTimeInSeconds()
{
    
            //part of the copy protection. 60 is actually coded into the unlock code
            int i_sixty = 60;
            
#ifdef _USES_PROTECTION
			if (app_glo.GetTrialDaysLeft() < 0)
            {
                i_sixty = cls_pro.i_max_players;
            }
#endif
            
            long l_temp = m_l_reschedule_raw;
            if (GetRescheduleType() == C_TRIGGER_TIME_MINUTES) l_temp *= i_sixty;
            if (GetRescheduleType() == C_TRIGGER_TIME_HOURS) l_temp *= i_sixty*i_sixty;
            if (GetRescheduleType() == C_TRIGGER_TIME_DAYS) l_temp *= i_sixty*i_sixty*24;
          
    return l_temp;
}

bool CTrigger::TimeTriggered(CTime *p_time, CEvent *pEvent)
{
    if (m_i_trigger_type != C_TRIGGER_TIME) return false; //um, no
    
    //are we supposed to do something now based on time?
    //first get the difference in time and convert that to seconds
    CTimeSpan elapsedTime = CTime(m_time) - *p_time;
    if (elapsedTime.GetTotalSeconds() <= 0)
    {
        //time is up.
     
      bool b_run_it = true;  
         if ( GetIgnoreLate())
        {
            //don't run if the actual time isn't within five minutes of this
            int i_ignore_time = 60*5; //five minutes;
            if (int(abs( int(elapsedTime.GetTotalSeconds()) )) >  i_ignore_time)
            {
                b_run_it = false; //don't run it

            }
        }
  
        //should we rechedule the next time check?
        if (m_b_reschedule_active)
        {
         
            long l_temp = GetRescheduleTimeInSeconds();
            int i_count = 0;
            do 
            {
            
                m_time += l_temp;
                elapsedTime= CTime(m_time) - *p_time;
                if (++i_count > 1000000)
                {
                    //emergency failsafe
                    log_error("Warning:  Reschedule time may be activated but set to 0. (%s)", uni(pEvent->GetName()).to_st());
                    break;
                }

            } 
            while( elapsedTime.GetTotalSeconds() <= 0);
        }
                   
       

        return b_run_it;
    }   
    return false;
}

bool CTrigger::HotkeyPressed(char vk, bool b_shift, bool b_ctrl, bool b_alt)
{
    if (vk != m_vk_key) return false;

    if (b_ctrl != m_b_ctrl) return false;
    if (b_alt != m_b_alt) return false;
    if (b_shift != m_b_shift) return false;

    return true;
}



bool CTrigger::Save(FILE * fp)
{
   //first save size
   	int i_size = sizeof(CTrigger);
  
    //first write the size of this struct
  	fwrite(&i_size, sizeof(int), 1, fp);

    //now write the trigger
      if (fwrite(this, i_size, 1, fp) <= 0)
	{
	  log_error("Error writing trigger info");
	}

         //great, but hold everything. if our buffer isn't null we need to save that too
     
      m_i_tchar_count_with_null = 0;
      if (this->GetBufferPointer())
      {
         m_i_tchar_count_with_null = wcslen(GetBufferPointer())+1; //one extra for the null
      }

       
     if (m_i_tchar_count_with_null > 0)
     {
         if (fwrite(GetBufferPointer(), 2, m_i_tchar_count_with_null, fp) <= 0)
         {
             log_error("Error writing trigger buffer info");
         }

     }

    return true;
}

bool CTrigger::Load(FILE * fp)
{
   
    //if we had a buffer allocated, better get rid of it
    SAFE_DELETE_ARRAY(m_p_buffer);

    
    //first load size
    int i_size = 0;
   	fread(&i_size, sizeof(int), 1, fp);

#ifdef _DEBUG
	if (i_size != sizeof(CTrigger))
	{
		LogMsg(_T("Converting old trigger from size %d to the new size %d"),
			i_size, sizeof(CTrigger));
	}
#endif
	//now read the trigger
      if (fread(this, i_size, 1, fp) <= 0)
	{
	  log_error("Error reading trigger info");
      return true;
	}
      
    //clear bad pointers
      m_p_buffer = NULL;

      //we may need to load buffer stuff
    
      if (m_i_tchar_count_with_null > 0)
      {
          //init memory for it
          m_p_buffer= (WCHAR*) new WCHAR[m_i_tchar_count_with_null];
          //we need to actually load it in
         if (fread(m_p_buffer, 2, m_i_tchar_count_with_null, fp) <= 0)
         {
             log_error("Error loading trigger text buffer info");
         }

      }


    return true;


}
