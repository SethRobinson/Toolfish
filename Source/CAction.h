#pragma once

#include "CGlobals.h"
#include "CDlgActionMessage.h"
#include "CDlgActionSound.h"
#include "CDlgActionWebsite.h"  
#include "CDlgActionCompare.h"
#include "CDlgActionRun.h"
#include "DlgActionStealth.h"
#include "CDlgActionEmail.h"
#include "CDlgActionToggle.h"
#include "CDlgActionAtomic.h"
#include "CDlgActionTTS.h"
#include "CDLGActionSmartMute.h"
#include "CDLGActionVolume.h"
#include "CDlgActionMonitor.h"
#include "CDlgActionSend.h"
#include "CDlgActionDelay.h"
#include "CDlgActionReboot.h"
#include "CDlgActionLeet.h"
#include "DlgMicVolume.h"
#include "CDlgActionRez.h"

const int C_ACTION_MAX_NAME_SIZE = 50;

const int C_FILTER_FIELD_SUBJECT = 0;
const int C_FILTER_FIELD_SENDER = 1;
const int C_FILTER_FIELD_RECIPIENT = 2;
const int C_FILTER_FIELD_BODY = 3;

const int C_FILTER_IGNORE_EMAILS_WITH = 0; //if this email has the text...
const int C_FILTER_IGNORE_EMAILS_WITHOUT = 1; //if this email doesn't have the text

const int C_ACTION_PROCESS_EXIST = 0;
const int C_ACTION_PROCESS_NOT_EXIST = 1;

const int C_ACTION_PROCESS_FILENAME = 0;  //where to search for a match
const int C_ACTION_PROCESS_WINDOW_TEXT = 1;

//for the control window action

//don't change the order, only add new ones at the end
enum
{
    C_ACTION_WM_QUIT = 0,
        C_ACTION_WM_MINIMIZE, 
        C_ACTION_WM_FOCUS,
        C_ACTION_WM_QUIT_HARD,
        C_ACTION_WM_OK,
        C_ACTION_WM_CANCEL,
        C_ACTION_WM_MAXIMIZE,
		C_ACTION_WM_MOUSE_LEFT_CLICK
        
};


const int C_ACTION_MESSAGE_NORMAL = 0;
const int C_ACTION_MESSAGE_KEY = 1;
const int C_ACTION_MESSAGE_CUSTOM = 2;
                

//for the reboot action

const int C_ACTION_REBOOT_LOGOFF = 0;
const int C_ACTION_REBOOT_SHUTDOWN = 1;
const int C_ACTION_REBOOT_RESTART = 2;


class ActionOptions
{
public:
    TCHAR st_name[45];
   
};

enum eActionTypes
{
	C_ACTION_MESSAGE = 0,
	C_ACTION_SOUND = 1,
	C_ACTION_WEBSITE = 2,
	C_ACTION_COMPARE = 3,
	C_ACTION_RUN = 4,
	C_ACTION_STEALTH = 5,
	C_ACTION_EMAIL = 6,
	C_ACTION_TOGGLE = 7,
	C_ACTION_ATOMIC = 8,
	C_ACTION_TTS = 9,
	C_ACTION_SMARTMUTE = 10,
	C_ACTION_VOLUME = 11,
	C_ACTION_MONITOR = 12,
	C_ACTION_SEND = 13,
	C_ACTION_DELAY = 14,
	C_ACTION_REBOOT = 15,
	C_ACTION_LEET = 16,
	C_ACTION_MIC_VOLUME = 17,
    C_ACTION_CHANGE_REZ = 18,

    //Add more above
    C_ACTION_TYPES_COUNT
};



const int C_ACTION_NO_PING_DATA = 10000000; //impossibe to ever get this ping

class CAction;

class CActionInfo
{
public:
    ActionOptions m_action[C_ACTION_TYPES_COUNT];
    int GetCount(){return C_ACTION_TYPES_COUNT;}
       CActionInfo()
    {
        _tcscpy(m_action[C_ACTION_MESSAGE].st_name, _T("Say/Show/Email/Macro Message"));
        _tcscpy(m_action[C_ACTION_SOUND].st_name, _T("Play Sound"));
        _tcscpy(m_action[C_ACTION_WEBSITE].st_name, _T("Open Website"));
        _tcscpy(m_action[C_ACTION_COMPARE].st_name, _T("Web Compare"));
        _tcscpy(m_action[C_ACTION_RUN].st_name, _T("Run/Open file (program, MP3, MPG, etc)"));
        _tcscpy(m_action[C_ACTION_STEALTH].st_name, _T("Stealth Mode Toggle"));
        _tcscpy(m_action[C_ACTION_EMAIL].st_name, _T("Check Email"));
        _tcscpy(m_action[C_ACTION_TOGGLE].st_name, _T("Open/Hide Toolfish Toggle"));
        _tcscpy(m_action[C_ACTION_ATOMIC].st_name, _T("Atomic Clock Time Adjust"));
        _tcscpy(m_action[C_ACTION_TTS].st_name, _T("Enable/Disable Toolfish's Text To Speech"));
        _tcscpy(m_action[C_ACTION_SMARTMUTE].st_name, _T("Enable/Disable Smart Mute"));
        _tcscpy(m_action[C_ACTION_VOLUME].st_name, _T("Set System Volume"));
        _tcscpy(m_action[C_ACTION_MONITOR].st_name, _T("Site Monitor"));
        _tcscpy(m_action[C_ACTION_SEND].st_name, _T("Send Command To Application"));
        _tcscpy(m_action[C_ACTION_DELAY].st_name, _T("Delay"));
        _tcscpy(m_action[C_ACTION_REBOOT].st_name, _T("Logoff/Shutdown/Reboot"));
        _tcscpy(m_action[C_ACTION_LEET].st_name, _T("Leet-Type Toggle"));
		_tcscpy(m_action[C_ACTION_MIC_VOLUME].st_name, _T("Set Mic Input Volume"));
        _tcscpy(m_action[C_ACTION_CHANGE_REZ].st_name, _T("Reset screen resolutions (NVidia fix)"));

	   }

       CDialog * CreateActionDialog(int i_action_type, CAction *p_action)
       {
           CDialog *p_dlg = NULL;

           if (i_action_type == C_ACTION_MESSAGE)
           {
               p_dlg = new CDlgActionMessage();
               //set it up 
               ((CDlgActionMessage*)p_dlg)->SetActionPointer(p_action);
           } else
              if (i_action_type == C_ACTION_SOUND)
           {
               p_dlg = new CDlgActionSound();
               //set it up 
               ((CDlgActionSound*)p_dlg)->SetActionPointer(p_action);
           }  else
    
            if (i_action_type == C_ACTION_WEBSITE)
           {
               p_dlg = new CDlgActionWebsite();
               //set it up 
               ((CDlgActionWebsite*)p_dlg)->SetActionPointer(p_action);
           }  else
    
            if (i_action_type == C_ACTION_COMPARE)
           {
               p_dlg = new CDlgActionCompare();
               //set it up 
               ((CDlgActionCompare*)p_dlg)->SetActionPointer(p_action);
           }  else
         if (i_action_type == C_ACTION_RUN)
           {
               p_dlg = new CDlgActionRun();
               //set it up 
               ((CDlgActionRun*)p_dlg)->SetActionPointer(p_action);
           }  else

            if (i_action_type == C_ACTION_STEALTH)
           {
               p_dlg = new CDlgActionStealth();
               //set it up 
               ((CDlgActionStealth*)p_dlg)->SetActionPointer(p_action);
           }  else
           if (i_action_type == C_ACTION_EMAIL)
           {
               p_dlg = new CDlgActionEmail();
               //set it up 
               ((CDlgActionEmail*)p_dlg)->SetActionPointer(p_action);
           }  else
           if (i_action_type == C_ACTION_TOGGLE)
           {
               p_dlg = new CDlgActionToggle();
               //set it up 
               ((CDlgActionToggle*)p_dlg)->SetActionPointer(p_action);
           }  else
           if (i_action_type == C_ACTION_ATOMIC)
           {
               p_dlg = new CDlgActionAtomic();
               //set it up 
               ((CDlgActionAtomic*)p_dlg)->SetActionPointer(p_action);
           }  else
      
            if (i_action_type == C_ACTION_TTS)
           {
               p_dlg = new CDlgActionTTS();
               //set it up 
               ((CDlgActionTTS*)p_dlg)->SetActionPointer(p_action);
           }  else
      
            if (i_action_type == C_ACTION_SMARTMUTE)
           {
               p_dlg = new CDlgActionSmartMute();
               //set it up 
               ((CDlgActionSmartMute*)p_dlg)->SetActionPointer(p_action);
           }  else
      
        if (i_action_type == C_ACTION_VOLUME)
           {
               p_dlg = new CDlgActionVolume();
               //set it up 
               ((CDlgActionVolume*)p_dlg)->SetActionPointer(p_action);
           }  else
			   if (i_action_type == C_ACTION_MIC_VOLUME)
			   {
				   p_dlg = new DlgMicVolume();
				   //set it up 
				   ((DlgMicVolume*)p_dlg)->SetActionPointer(p_action);
			   }  else
                   if (i_action_type == C_ACTION_CHANGE_REZ)
                   {
                       p_dlg = new CDlgActionRez();
                       //set it up 
                       ((CDlgActionRez*)p_dlg)->SetActionPointer(p_action);
                   }
                   else

        if (i_action_type == C_ACTION_MONITOR)
           {
               p_dlg = new CDlgActionMonitor();
               //set it up 
               ((CDlgActionMonitor*)p_dlg)->SetActionPointer(p_action);
           }  else
      
         if (i_action_type == C_ACTION_SEND)
           {
               p_dlg = new CDlgActionSend();
               //set it up 
               ((CDlgActionSend*)p_dlg)->SetActionPointer(p_action);
           }  else
     if (i_action_type == C_ACTION_DELAY)
           {
               p_dlg = new CDlgActionDelay();
               //set it up 
               ((CDlgActionDelay*)p_dlg)->SetActionPointer(p_action);
           }  else
    
       if (i_action_type == C_ACTION_REBOOT)
           {
               p_dlg = new CDlgActionReboot();
               //set it up 
               ((CDlgActionReboot*)p_dlg)->SetActionPointer(p_action);
           }  else
           if (i_action_type == C_ACTION_LEET)
           {
               p_dlg = new CDlgActionLeet();
               //set it up 
               ((CDlgActionLeet*)p_dlg)->SetActionPointer(p_action);
           }  else
      
           {
             LogError(_T("Don't know how to handle this kind of ACTION."));
            
           }

           return p_dlg;
       }

};

        
extern CActionInfo g_actions;


class CAction
{
public:

    CAction();

    ~CAction();
     
     CDialog* InitActionDialog();
     bool Save(FILE * fp);
     bool Load(FILE * fp);
     void CopyToBuffer(const WCHAR * p_wchar);
     WCHAR * GetBufferPointer(); 
     TCHAR * GetName(bool b_long);
     void SetActionType(int i_new){
         
         m_i_action_type = i_new;
     
         if (m_i_action_type == C_ACTION_RUN)
         {
             m_b_no_wait = true;
         }

     if (m_i_action_type == C_ACTION_EMAIL)
     {
       m_b_say_count = true;
       m_b_say_from = true;
       m_b_say_subject = true;
     }

     if (m_i_action_type == C_ACTION_VOLUME)
     {
         SetVolume(10); //default should be max
         SetTTS(false);
         SetFade(true);
     }

	 if (m_i_action_type == C_ACTION_MIC_VOLUME)
	 {
		 SetVolume(100); //default should be max
	 }

     if (m_i_action_type == C_ACTION_MONITOR)
     {
         SetPort(80); //default port for the monitoring
         SetWebPage(true);
         SetMonitorError(true);
         SetMonitorSize(false);
         SetMonitorSizeBytes(1024);
         SetMonitorTimeout(true);
         ClearMonitorStats();
     }
       if (m_i_action_type == C_ACTION_SEND)
       {
           //defaults for the send to window event
           CopyToBuffer(L"something.exe");
           SetShift(false);
           SetCtrl(false);
           SetAlt(false);
           SetVK(VK_RETURN); //enter
           
       }
       if (m_i_action_type == C_ACTION_DELAY)
       {
           SetDelay(1000); //let's default to one second
       }


     }
     int GetActionType(){return m_i_action_type;}
     void SetFilename( const WCHAR *p_new);
     WCHAR * GetFilename(){return m_filename;}
     //these are so simply to access I'll let them be public
     WCHAR * GetPassword(){return m_wst_password;}
     void SetPassword(const WCHAR * p_new){wcscpy(m_wst_password, p_new);}

     WCHAR * GetLogon(){return m_wst_logon;}
     void SetLogon(const WCHAR * p_new){wcscpy(m_wst_logon, p_new);}
     
     WCHAR * GetEmailServer(){return m_wst_server;}
     void SetEmailServer(const WCHAR * p_new){wcscpy(m_wst_server, p_new);}

     WCHAR * GetReturnEmail(){return m_wst_return_email;}
     void SetReturnEmail(const WCHAR * p_new){wcscpy(m_wst_return_email, p_new);}

     WCHAR * GetEmailSubject(){return m_wst_subject;}
     void SetEmailSubject(const WCHAR * p_new){wcscpy(m_wst_subject, p_new);}
    WCHAR * GetToEmail(){return m_wst_to_email;}
    WCHAR * GetEmailFile(){return m_wst_attached_file;}
    void SetEmailFile(const WCHAR * p_new){wcscpy(m_wst_attached_file, p_new);}
    void SetToEmail(const WCHAR * p_new){wcscpy(m_wst_to_email, p_new);}
     void SetTTS(bool b_new){m_b_content = b_new;} //reusing this var for the TTS action
     bool GetTTS(){return m_b_content;} //reusing this var for the TTS action
     int GetVolume(){return m_i_filter_ignore_type;} //reusing this for the Set System Volume action
     void SetVolume(int i_new){m_i_filter_ignore_type = i_new;}
     bool GetFade(){return m_b_words_found;} //reusing this for the fade of set system vol action
     void SetFade(bool b_new){m_b_words_found = b_new;}

     //for Server monitor action, I reuse a lot of vars... 
     void SetPort(int i_new){m_i_port = i_new;}
     int GetPort(){return m_i_port;}
     void SetWebPage(bool b_new){m_b_content = b_new;}
     bool GetWebPage() {return m_b_content;} //is this monitoring a webpage or not?
     void SetMonitorSize(bool b_new){ m_b_say_count = b_new;} //do we care about what size the content was?
     bool GetMonitorSize(){return m_b_say_count;} //do we care about content size?
     void SetMonitorSizeBytes(int i_new){ m_i_filter_ignore_type = i_new;}
     int GetMonitorSizeBytes() {return m_i_filter_ignore_type;}
     void SetMonitorTimeout(bool b_new){m_b_date = b_new;}
     bool GetMonitorTimeout(){return m_b_date;}
     void SetMonitorError(bool b_new){m_b_macro = b_new;}
     bool GetMonitorError(){return m_b_macro;}
     bool GetMonitorReverse(){return m_b_email;}
     void SetMonitorReverse(bool b_new) {m_b_email = b_new;}
     bool GetWebCompareAuthenticate(){return m_b_email;}
     void SetWebCompareAuthenticate(bool b_new) {m_b_email = b_new;}

     void ClearMonitorStats(); //clear out all monitor stats
     void SetSuccess(){m_i_monitor_success++;}
     void SetFail(){m_i_monitor_fail++;}
     void AddPing(int i_ms);
     int GetAveragePing();
     int GetLowPing(){ if (m_i_min_ping == C_ACTION_NO_PING_DATA) return 0; return m_i_min_ping;}
     int GetHighPing(){if (m_i_max_ping == C_ACTION_NO_PING_DATA) return 0;return m_i_max_ping;}
     float CAction::GetUptime();
     int GetMonitorFails(){return m_i_monitor_fail;}
     int GetMonitorSamples(){return m_i_monitor_fail+m_i_monitor_success;}
     
     //for sending messages to applications action
     int GetProcessSearchField(){return m_i_min_ping;}
     void SetProcessSearchField(int i_new) {m_i_min_ping = i_new;}
      int GetProcessWMCommand(){return m_i_port;}
     void SetProcessWMCommand(int i_new) {m_i_port = i_new;}
     void SetProcessMessageType(int i_new){m_i_monitor_success = i_new;}
     int GetProcessMessageType(){return m_i_monitor_success;}
     bool GetAlt(){return m_b_content;}
     void SetAlt(bool b_new) {m_b_content = b_new;}
     bool GetCtrl(){return m_b_log;}
     void SetCtrl(bool b_new) {m_b_log = b_new;}
     bool GetShift(){return m_b_say;}
     void SetShift(bool b_new) {m_b_say = b_new;}
     void SetVK(char ch_new) {m_c_vkey = ch_new;}
     char GetVK(){return m_c_vkey;}
     UINT GetCustomMsg() {return m_ui_msg;}
     void SetCustomMsg(UINT ui_new){m_ui_msg = ui_new;}
     DWORD GetCustomLParam(){return m_dw_lparam;}
     void SetCustomLParam(DWORD dw_new){m_dw_lparam = dw_new;}
     DWORD GetCustomWParam(){return m_dw_wparam;}
     void SetCustomWParam(DWORD dw_new){m_dw_wparam = dw_new;}
//delay
     void SetDelay(int i_ms){m_i_filter_ignore_type = i_ms;}
     int GetDelay(){return m_i_filter_ignore_type;}

     void SetRebootType(int i_ms){m_i_filter_ignore_type = i_ms;}
     int GetRebootType(){return m_i_filter_ignore_type;}

     bool GetOpenInSameWindow(){return m_b_popup;}
     void SetOpenInSameWindow(bool b_new){m_b_popup = b_new;}
     

     CHARFORMATA m_cf; //font info for the richedit controller
     bool m_b_log; //show this message on  the log
     bool m_b_say; //say it with text to speech software
     bool m_b_macro; //send it to hot window
     bool m_b_popup;

     
private:
    
    char m_st_name[C_ACTION_MAX_NAME_SIZE];
    int m_i_action_type;
    WCHAR *m_p_buffer; //tchar buffer
    WCHAR m_filename[C_MAX_URL_SIZE]; //some actions need this
 
public:
    //for webcompare stuff

     bool m_b_content; //compare webpages by contents
     bool m_b_date; //compare by date
     bool m_b_words_found; //check for certain words
     bool m_b_words_not_found; //if words are missing, flag compare

     //as for the words themselves.. hmm..
     WCHAR m_ws_words_found[128];
     WCHAR m_ws_words_not_found[128];

     float m_f_content_percent; //how much change has to occur to flag a change
     bool m_b_no_wait; //shoudl we wait as the process is run?  (used in  the run action)
     WCHAR m_wst_server[256];
     WCHAR m_wst_logon[128];
     WCHAR m_wst_password[50];
     WCHAR m_wst_return_email[256];
     WCHAR m_wst_subject[256]; //also used as filter_text
     WCHAR m_wst_to_email[256];

     bool m_b_say_count;
     bool m_b_say_from;
     bool m_b_say_subject;
     bool m_b_email; //if true, we need to send an email

     //for filtering email with the email check action
     bool m_b_filter_active; 
     int m_i_filter_ignore_type;
     int m_i_filter_field;
     int m_i_port;

     int m_i_monitor_success;
     int m_i_monitor_fail;
     int m_i_total_ping;
     int m_i_max_ping;
     int m_i_min_ping;
     int m_i_ping_count; //how many samples we've taken total
     WCHAR m_wst_attached_file[256];
     char m_c_vkey;
     UINT m_ui_msg;
     DWORD m_dw_lparam;
     DWORD m_dw_wparam;
};
