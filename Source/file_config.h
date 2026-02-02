
 #pragma once

//******* SET THESE ********

#define D_LOAD_FUNCTION_NAME FileConfigLoad
#define D_SAVE_FUNCTION_NAME FileConfigSave




//file name
#define D_FILE_NAME "config.dat"



const int C_TIMER_TYPE_MINUTES = 1;
const int C_TIMER_TYPE_SECONDS = 0;
//********************************


#ifndef _UNLOCK_UTILITY_
#include "stdafx.h"
#endif

#include "CVoice.h"
#include "CInputStats.h"
//never change the contents of this struct

struct CFileConfigLoader
{
	float f_version; //version number
	int i_file_size; //easier to do upgrades
};

//customize as needed
//little helper class to process input data.  They will both get initted in CConfig so
//we won't worry about the constructor, it could screw up our loading.


class CConfig
{


public:

     enum LoginMethod
    {
        NoLoginMethod=0,
            CramMD5Method=1,
            AuthLoginMethod=2,
            LoginPlainMethod=3
    };
    
    
    float f_version; //version number
  	int i_file_size; //easier to do upgrades

    bool m_b_mute_enabled;
    bool m_b_trim_log;
    bool m_b_log_active; //log everything?
    WCHAR  m_st_key_log[MAX_PATH];
    bool m_b_key_log_enabled;
    bool m_b_boot_load;
    int m_i_mute_volume;
    bool m_b_smart_mute; //is smart muting enabled?
    int m_i_mute_time_type; //seconds, minutes, what?
    int m_i_mute_time; //how many somethings we have to wait
    bool m_b_log_programs_run; 
    int m_i_trim_lines; //how many lines toolfish_log.txt should be at startup
    char m_st_code[50]; //registration code.  null is none is entered
    char m_st_unlock_key[50]; //unlock key,  they use this to get the real code
    bool m_b_showed_welcome;
    CVoice m_voice; //info about the default voice. 
    bool m_b_disable_tts;
    bool m_b_boot_stealth; //if true, no icon is display on boot loading
    WCHAR m_st_email[256];
    WCHAR m_st_smtp_server[256];
    bool m_b_tray_disable_blinking;
    bool m_b_tray_no_trigger_on_email;
    bool m_b_tray_no_trigger_on_webcompare;
  
    //add some new stuff
    bool m_b_smtp_mime; //use mime on all emails?
    bool m_b_smtp_use_auth; //if yes, we'll do a fancy style logon
    WCHAR m_st_smtp_username[80];
    WCHAR m_st_smtp_password[80];
    LoginMethod m_smtp_logon; //enum type for the kind of authentication, delcared in smtp.h
    int m_i_smtp_port; //should be 25 

    
    CInputStats m_stats_today;
    CInputStats m_stats_total;

    time_t m_ctime_today;

    bool m_b_key_overlay_active;
    bool m_b_server_ip_disabled;
    bool m_b_minimize_on_close; //if true, clicking the close button will minimize

    CConfig()
    {
        //set defaults
        memset(this, 0, sizeof(CConfig));


    }
};

//these will get preprocessed into the correct declarations

bool D_LOAD_FUNCTION_NAME (CConfig *sys);
bool D_SAVE_FUNCTION_NAME (CConfig *sys);

