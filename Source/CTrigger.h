#pragma once
#include "CGlobals.h"
#include "CDlgTriggerTime.h"
#include "CDlgTriggerHotkey.h"
#include "CDlgTimeRange.h"
#include "CDlgTriggerInactivity.h"
#include "CDlgTriggerDayFilter.h"
#include "CDlgTriggerProcess.h"

class CEvent;
 

const int C_TRIGGER_MAX_NAME_SIZE = 50;
const int C_TRIGGER_TIME_SECONDS = 0;
const int C_TRIGGER_TIME_MINUTES = 1;
const int C_TRIGGER_TIME_HOURS = 2;
const int C_TRIGGER_TIME_DAYS = 3;


const int C_TRIGGER_PROCESS_EXIST = 0;
const int C_TRIGGER_PROCESS_NOT_EXIST = 1;

const int C_TRIGGER_PROCESS_FILENAME = 0;  //where to search for a match
const int C_TRIGGER_PROCESS_WINDOW_TEXT = 1;


class TriggerOptions
{
public:
    TCHAR st_name[40];
   
};

const int C_TRIGGER_TIME = 0;
const int C_TRIGGER_HOTKEY = 1;
const int C_TRIGGER_TIME_RANGE = 2;
const int C_TRIGGER_INACTIVITY = 3;
const int C_TRIGGER_DAY_FILTER = 4;
const int C_TRIGGER_PROCESS = 5;
const int C_TRIGGER_TYPES_COUNT = 6;

class CTriggerInfo
{
public:
    TriggerOptions m_trigger[C_TRIGGER_TYPES_COUNT];
    int GetCount(){return C_TRIGGER_TYPES_COUNT;}
       CTriggerInfo()
    {
        _tcscpy(m_trigger[0].st_name, _T("Time"));
         _tcscpy(m_trigger[1].st_name, _T("Hotkey"));
        _tcscpy(m_trigger[2].st_name, _T("Time Range"));
        _tcscpy(m_trigger[3].st_name, _T("Inactivity"));
        _tcscpy(m_trigger[4].st_name, _T("Day Filter"));
        _tcscpy(m_trigger[5].st_name, _T("If Process Exists"));
    }

    CDialog * CTriggerInfo::CreateTriggerDialog(int i_trigger_type, CTrigger *p_trigger);  

};

        
extern CTriggerInfo g_triggers;

class CTrigger
{
public:

    CTrigger();
    ~CTrigger()
    {
        //if we had a buffer allocated, better get rid of it
    SAFE_DELETE_ARRAY(m_p_buffer);
    }

     TCHAR * GetName();
     void SetTriggerType(int i_new)
     {
         m_i_trigger_type = i_new;
         if (m_i_trigger_type == C_TRIGGER_TIME_RANGE)
         {
             //special default settings we need
             //9 AM
             SetTime(&CTime(2002,1,1,9,0,0));
             //5 PM
             SetTimeEnd(&CTime(2002,1,1,17,0,0));
         }
        if (m_i_trigger_type == C_TRIGGER_INACTIVITY) 
        {
          SetRescheduleRaw(15);
          SetRescheduleType(C_TRIGGER_TIME_MINUTES);
        }  

        if (m_i_trigger_type == C_TRIGGER_PROCESS)
        {
            SetProcessSearchField(C_TRIGGER_PROCESS_FILENAME);
            SetProcessFoundField(C_TRIGGER_PROCESS_EXIST);
            CopyToBuffer(L"someapp.exe");
        }
     }
     int GetTriggerType(){return m_i_trigger_type;}
     CDialog* InitTriggerDialog(){return g_triggers.CreateTriggerDialog(m_i_trigger_type, this);};

     
     //TIME   ***********
     bool TimeTriggered(CTime *p_time, CEvent *pEvent);
	 CTime GetTime(){return CTime(m_time);};
	 void SetTime(CTime *p_time);
	 
     CTime GetTimeEnd(){return CTime(m_time_end);};
     void SetTimeEnd(CTime *p_time){m_time_end = *(__time64_t*) &p_time;}

     void SetRescheduleRaw(long i_new){m_l_reschedule_raw = i_new;}
     bool GetRescheduleActive(){return m_b_reschedule_active;}
     void SetRescheduleActive(bool b_new){m_b_reschedule_active = b_new;}
     void SetRescheduleType(int i_new){m_i_reschedule_type = i_new;}
     int GetRescheduleType(){return m_i_reschedule_type;}
     long GetRescheduleRaw(){return m_l_reschedule_raw;}
     
     
     bool CTrigger::TimeIsBetweenRange(CTime current_time);



     bool Save(FILE * fp);
     bool Load(FILE * fp);
     bool HotkeyPressed(char vk, bool b_shift, bool b_ctrl, bool b_alt);
     long CTrigger::GetRescheduleTimeInSeconds();

     bool GetIgnoreLate(){return m_b_alt;}
     void SetIgnoreLate(bool b_new) {m_b_alt = b_new;}
     
     void CopyToBuffer(const WCHAR * p_wchar);
     WCHAR * GetBufferPointer(); 
 
     //especially for the process trigger
     
     int GetProcessSearchField(){return m_i_reschedule_type;} //reusing this var..
     void SetProcessSearchField(int i_new){m_i_reschedule_type = i_new;} //reusing this var..
     int GetProcessFoundField() {return m_l_reschedule_raw;}  //reusing this var
     void SetProcessFoundField(int i_new){m_l_reschedule_raw = i_new;} 
     bool GetProcessFocus(){return m_b_pass_through;} //reused
     void SetProcessFocus(bool b_new){m_b_pass_through = b_new;}


private:
    int m_i_trigger_type; //-1 for none
    char m_st_name[C_TRIGGER_MAX_NAME_SIZE];
    time_t m_time_last_run;


    //for time
    time_t m_time; //same as t_time but gives us some free functions
    bool m_b_reschedule_active;
    long m_l_reschedule_raw; //this might be in minutes or hours or whatever
    int m_i_reschedule_type; //minutes, seconds or hours etc
public:
    //for hotkeys
    char m_vk_key; //key for hotkey
    bool m_b_alt; //does alt also need to be pressed?
    bool m_b_ctrl; //does ctrl also need to be pressed?
    bool m_b_shift; //does shift also need to be pressed?
private:
    time_t m_time_end; //same as t_time but gives us some free functions
public:
    bool m_b_check_for_activity; //false for activity
    bool m_b_pass_through; //used for hotkeys

    //for the day filter trigger
    bool m_b_a_day_active[7]; //days of the week
    WCHAR *m_p_buffer; //tchar buffer
    int m_i_tchar_count_with_null;
};

