//this is share by most files in the project
#pragma once

#include "stdafx.h"
#include "..\Shared\all.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\uni.h"
#include "SMDLL\lib1.h"
#include "shared_funcs.h"
#include "Source.h"
#include "..\Shared\CTimer.h" 
#include "security_online.h"
#include "CVoice.h"


#ifdef _UNICODE
void LogMsg(char * lpFormat, ...);

#endif
const int C_MAX_URL_SIZE = 2024;

const float C_F_VERSION = 2.30f;

//these macros assume 1 equals a second, like with CTime

#define C_ONE_MINUTE 60
#define C_ONE_HOUR (C_ONE_MINUTE*60)
#define C_ONE_DAY (C_ONE_HOUR*24)
#define C_ONE_WEEK (C_ONE_DAY * 7)
#define C_ONE_MONTH (C_ONE_DAY * 30)


const int C_DEMO_TIME_MODIFIED = 22;
const int C_DEMO_TIME_SUBTRACT = 8; //some tricks so people can't just search for '14' to
//change the demo time in the .exe
#define C_DEMO_DAYS (C_DEMO_TIME_MODIFIED-C_DEMO_TIME_SUBTRACT); //but wouldn't the compiler just optimize it away?  not if we disable it for that part.. uh... shutup
#include "file_config.h"

extern CConfig glo;

extern CSource theApp;


class CSpeech; //forward for speed


const int C_MAX_LOG_LINES = 1000;
#include "assert.h"
const int WM_ADD_TO_LOG = WM_USER+200;
const int WM_ADD_TO_LOG_DLL = WM_USER+201;
const int WM_APP_OPENED = WM_USER+202;
const int WM_ADD_TO_LOGW = WM_USER+203;
const int WM_SAY = WM_USER+300;

const int WM_GOTKEY = WM_USER+500;
const int WM_GOTMOUSE = WM_USER+501;
const int WM_MOUSECLICK = WM_USER+502;
const int WM_MY_TRAY_NOTIFICATION = WM_USER+10;
const int WM_HIDE = WM_USER+11;
const int WM_TOGGLE_STEALTH = WM_USER+12;
const int WM_TOGGLE_TOOLFISH = WM_USER+13;
const int WM_TRAY_BLINK_ON = WM_USER+14;
const int WM_TRAY_BLINK_OFF = WM_USER+15;
const int WM_TOGGLE_TTS = WM_USER+16; //turn text to speech on or off

class CEventList; //forward for speed

const int C_MOUSE_SENSITIVITY = 3;
#include "CAction.h"

class CApplicationGlobals
{
private:
   TCHAR m_st_program_name[20];
   float m_f_version;
   HWND m_main_hWnd;
   HWND m_dialog_hWnd;
   CEventList *m_p_events;
   bool m_b_paused;
   CSpeech * m_p_speech;
   unsigned long m_ul_unmute_vol; //volume to set to when returning normal audio
   bool m_b_muted; //if true computer is currently muted by us
   CTimer m_timer_mute;
   int m_i_mouse_counter; //keep track of how many times the mouse was moved
   char m_st_ip[128];
   int m_g_threads_active;

   TCHAR m_ws_last_url_compared[C_MAX_URL_SIZE];
   HANDLE m_thread_mutex; //use this to insure we're threadsafe
   int m_i_trial_days_left;
   CTimer m_timer_security_check;
   CTimer m_timer_shut_down_message;
   int m_i_last_new_email_count;
   TCHAR m_st_home_dir[MAX_PATH]; //filled out upon startup
 
public:
    CApplicationGlobals(); //constructor
    CApplicationGlobals::~CApplicationGlobals();
    void SetPaused(bool b_new){m_b_paused = b_new;}
    bool GetPaused(){return m_b_paused;}
    UINT m_timer_ident; //0 if no timer is active
    void CApplicationGlobals::DecActiveThreadCount();

    TCHAR * GetAppName(){return (TCHAR*)&m_st_program_name;}
    float GetVersion(){return m_f_version;}
    HWND GetHWND(){return m_main_hWnd;}
    void SetHWND(HWND hwnd_new){m_main_hWnd = hwnd_new;}
    HWND GetDialogHWND(){return m_dialog_hWnd;}
    void SetDialogHWND(HWND hwnd_new){m_dialog_hWnd = hwnd_new;}
    void CApplicationGlobals::IncActiveThreadCount();
    int GetTrialDaysLeft(){return m_i_trial_days_left;}
    void SetTrialDaysLeft(int i_new){m_i_trial_days_left = i_new;}
    CEventList * GetEventList(){return m_p_events;}; 
    void SecurityCheck();
    CHARFORMAT cf_default_font;
    CSpeech * GetSpeech(){return m_p_speech;}
    void InitSpeech();
    void SetUnmuteVol(unsigned long ul_new){m_ul_unmute_vol = ul_new;}
    unsigned long GetUnmuteVol(){return m_ul_unmute_vol;}
    bool GetMuted(){return m_b_muted;}
    void SetMuted(bool b_new){m_b_muted = b_new;}
    bool TimeToMute();
    void ResetMuteTimer();
    void CApplicationGlobals::CleanUpMouseMovement();
    bool CApplicationGlobals::MouseMoved();
    void SetIP(char *p_new){strcpy((char*)&m_st_ip, p_new);};
    char * GetIP();
    TCHAR * GetLastURLCompared(){return m_ws_last_url_compared;}
    void SetLastURLCompared(TCHAR * ws_new){_tcscpy(m_ws_last_url_compared, ws_new);}
    int GetLastNewEmailCount(){return m_i_last_new_email_count;}
    void SetLastNewEmailCount(int i_new){m_i_last_new_email_count = i_new;}
    int GetActiveThreadCount();
    void ShutDownMessageCheck();
    void RetrieveIPAddress();
    TCHAR * GetHomeDir(){return (TCHAR*)&m_st_home_dir;}

       bool m_b_tampered; //true if trial date has been tampered with
      bool m_b_leave_message; //will give a message and shut down.
        bool m_b_keyboard_disabled; //yes when cleaning keyboard


      int GetSecondsInactive(){
//          log_msg("Have been inactive %d seconds.",m_timer_mute.get_elapsed_seconds_force() );
          return m_timer_mute.get_elapsed_seconds_force();
      }
      
      
      void CApplicationGlobals::ActivateSpeech(bool b_turn_on);

};


extern CApplicationGlobals app_glo;

void SetTrayBlinking(bool b_blink_on, long i_code);



