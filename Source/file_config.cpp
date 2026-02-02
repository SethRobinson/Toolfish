#include "stdafx.h"
#include "file_config.h"
#ifndef _UNLOCK_UTILITY_
#include "CGlobals.h"
#endif

bool D_LOAD_FUNCTION_NAME (CConfig *sys)
{
 	memset(sys,0,sizeof(CConfig)); //zero out everything
	//Set any defaults
	
	FILE * fp;

    if ( (fp=fopen(D_FILE_NAME, "rb")) == NULL)
    {
        //Not able to create sys file. Let's set the defaults
        log_msg("Can't load %s file, creating a new one with defaults.", D_FILE_NAME);
        //version and filesize will be set on save each time.
        //set some defaults
        sys->m_b_key_log_enabled = false;
        sys->m_b_trim_log = true;
        wcscpy(sys->m_st_key_log, L"keystrokes.txt");
#ifndef _UNLOCK_UTILITY_
        sys->f_version = C_F_VERSION;
#else
        sys->f_version = 2.02f;
        
#endif
        sys->i_file_size = sizeof(CConfig);
        sys->m_b_boot_load = true;
        sys->m_b_smart_mute = false;
        sys->m_i_mute_volume = 2;
        sys->m_i_mute_time = 15;
        sys->m_i_mute_time_type = C_TIMER_TYPE_MINUTES;
        sys->m_b_log_programs_run = 0;
        
        // Volume knob sensitivity defaults
        sys->m_b_volume_knob_sensitivity = false;
        sys->m_i_volume_knob_multiplier = 2;  // 2 extra steps = 4% extra per press
        sys->m_b_volume_knob_shift_only = false;
        sys->m_i_trim_lines = 200;
        sys->m_voice.SetDefaults();
        wcscpy(sys->m_st_smtp_server, L"mail");
        wcscpy(sys->m_st_email, L"Toolfish <toolfish@yourdomain.com>");
        sys->m_smtp_logon = CConfig::AuthLoginMethod;
        sys->m_i_smtp_port = 25;
        

        return true; //no error
    }
    
	//load header
	CFileConfigLoader sys_loader;

	if (fread(&sys_loader, sizeof(CFileConfigLoader), 1, fp) <= 0)
	{
	  log_error("Couldn't read %s header.  Corrupted file?  Delete it maybe.",
		  D_FILE_NAME);
	  fclose(fp);
	  return false;
	}

	char st_crap[255];
	sprintf(st_crap, "Loading file %s. (version %.3f)",D_FILE_NAME, sys_loader.f_version);
	log_msg(st_crap);

	//Load real thing.
	if (fseek(fp, 0, SEEK_SET ))
	{
	 log_error("Unable to perform seek on config.dat file.");
	}
	if (!fread(sys, sys_loader.i_file_size, 1,fp))
	{
	 log_error("Error with fread loading config.dat file.");
	}

	fclose(fp);
    //fix any bad data

    if (sys->m_smtp_logon == CConfig::NoLoginMethod)
    {
        sys->m_smtp_logon = CConfig::AuthLoginMethod;
    }
    if (sys->m_i_smtp_port == 0) sys->m_i_smtp_port = 25;
    return true;
}


bool D_SAVE_FUNCTION_NAME (CConfig *sys)
{
	
	//Set some values
#ifndef _UNLOCK_UTILITY_
    sys->f_version = C_F_VERSION;
#else
    sys->f_version = 2.02f;
    
#endif
 sys->i_file_size = sizeof(CConfig);


 //Now let's actually save the bitch.
 FILE * fp;
     if ( (fp=fopen(D_FILE_NAME, "wb")) == NULL)
	 {
	  log_error("Error saving %s.  Out of HD space or something?", D_FILE_NAME);
	  return false;
	 }

	 if (fwrite(sys, sizeof(CConfig), 1, fp) <= 0)
	 {
		log_error("Wrote zero bytes to %s file?  Something is wrong.",
			D_FILE_NAME);
		fclose(fp);
		return false;
	 }
  
 	//done saving.
	 fclose(fp);
//	 log_msg("sys file saved.");
	 return true;


}
