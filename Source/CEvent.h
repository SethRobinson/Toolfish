#pragma once

#include "CTrigger.h"
#include "CAction.h"

#include <vector>

using namespace std;

typedef vector<CTrigger*> TriggerVec;
typedef vector<CAction*> ActionVec;


const int C_EVENT_MAX_NAME_SIZE = 100;

#ifndef _UNICODE
extern char g_event_name_buff[C_EVENT_MAX_NAME_SIZE];
#endif

class CEvent
{
public:
    CEvent();
    ~CEvent();
   
    WCHAR * GetName(){return (WCHAR*)&m_st_name;}
    TCHAR * GetAutoName(bool b_long);
    TCHAR * GetSmartName()
    {
        if (GetAutoNameEnabled()) return GetAutoName(false); else 
        {
#ifdef _UNICODE
            return GetName();
#else
            return uni(GetName()).to_st();
            
#endif
        }
    }
    TCHAR * GetSmartNameLong(){if (GetAutoNameEnabled()) return GetAutoName(true); else
           {
#ifdef _UNICODE
        return GetName();
#else
        strcpy(g_event_name_buff,uni(GetName()).to_st() ) ;
        return (char*)&g_event_name_buff;

#endif
    }
}
    
    void SetName(const WCHAR *p_st_name)
    {
        
        wcscpy(m_st_name, p_st_name);
    
    }
    bool GetEnabled(){return m_b_enabled;}
    void SetEnabled(bool b_new){m_b_enabled = b_new;}
    TCHAR * GetStatus();
    void InitTrigger();
    void KillTrigger();
    void DeleteTrigger(int i_index);
    int GetTriggerCount();
    CTrigger * GetTrigger(int i_index);
    int AddTrigger();
    bool Save();
    bool Load(TCHAR p_filename[]);
    CTrigger * FindTrigger(int i_trigger_type);
    CAction * FindAction(int i_action_type);
    void InitAction();
    void KillAction();
    void DeleteAction(int i_index);
    int GetActionCount();
    CAction * GetAction(int i_index);
    int AddAction();
    int CountActiveTriggers();
    int CountActiveActions();
    int GetIndex(){return m_i_index;}
    void SetIndex(int i_new){m_i_index = i_new;}
    bool GetAutoNameEnabled(){return m_b_auto_name;}
    void SetAutoNameEnabled(bool b_new){m_b_auto_name = b_new;}
    bool GetQuietMode(){return m_b_quiet;}
    void SetQuietMode(bool b_new){m_b_quiet = b_new;}
    WCHAR * GetCategoryName();
    void SetAutoDeleteAfterRunning(bool b_new) {m_b_auto_delete_after_running = b_new;}
    bool GetAutoDeleteAfterRunning(){return m_b_auto_delete_after_running;}
    void SetAutoDeletePermission(bool b_new) {m_b_delete_permission_granted = b_new;}
    bool GetAutoDeletePermission(){return m_b_delete_permission_granted;}
    
    void SetFileName(const WCHAR *wst_filename){wcscpy(m_st_filename, wst_filename);};
    WCHAR * GetFileName(){return m_st_filename;}
    CTime * GetTimeLastRun(){return (CTime*)&m_time_last_run;}
	CTime GetTimeLastRunAsCTime(){return CTime (m_time_last_run);}
    void SetLastRunTimeToNow()
    {
        m_time_last_run = static_cast<time_t>(CTime::GetCurrentTime().GetTime());
    }

    void DeleteEventFile();
    int MoveTriggerDown(int i_index);
    int MoveTriggerUp(int i_index);

    int MoveActionDown(int i_index);
    int MoveActionUp(int i_index);
    void SetCategoryName(WCHAR *p_text);
    void SetCategoryName(CHAR *p_ansi_text);
    void SetCategoryNameBasedOnEventType();

private:
    
    void CEvent::KillCategory();
   
   WCHAR m_st_name[C_EVENT_MAX_NAME_SIZE];
   int m_i_trigger_count;
   int m_i_action_count;
    bool m_b_enabled;
    bool m_b_run_once;
   //pointers to be filled with data
   TriggerVec *m_p_vec_trigger;
   ActionVec *m_p_vec_action;
   int m_i_index; //what we are in the main CEventList index, helpful for saving
   bool m_b_auto_name; //if yes, we don't use the stored name but generate our own
   WCHAR m_st_filename[MAX_PATH];
   float m_f_version; //version of Toolfish this was last saved with
   time_t m_time_last_run;
   bool m_b_quiet; 

   //new as of V2.00
   int m_i_category_size; //size in WCHAR's of our category.  0 alerts us that an old
   //toolfish version data was loaded.
   WCHAR *m_p_wst_category; //NULL if not used
   bool m_b_auto_delete_after_running; //if true, this event will self distruct after being run.
   bool m_b_delete_permission_granted; //if false, auto_delete will never occur.  This way
   //test runs can be done without the event getting deleted.
};
