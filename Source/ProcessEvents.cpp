#include "stdafx.h"
#include "ProcessEvents.h"
#include "CEvent.h"
#include "CEventList.h"          
#include "Mmsystem.h."
#include "mute_util.h"
#include "CDlgMute.h"
#include <winuser.h>      
#include "..\Shared\CTextParse.h"
#include "internet_util.h"
#include "spy_util.h"
#include <errno.h>
#include "mail_util.h"
#include "AtomicClock.h"
#include "CDlgAlert.h"
#include "ServerMonitor.h"
#include "ProcessFindUtils.h"
#include "reboot_util.h"
#include "process.h"
#include <direct.h>
#include "MicAdjust.h"
#include "SoundOutputUtil.h"


extern bool g_b_tray_active;


void GlobalSay(const TCHAR *p_message)
{
    int i_length = _tcslen(p_message)+1; //don't forget the null
    TCHAR *p_send = new TCHAR[i_length];
    _tcscpy(p_send, p_message);
    SendMessage(app_glo.GetHWND(), WM_SAY, 0, (LONG)p_send);

    //hopefully the messagesd will be received by the main window and the memory
    //will be freed there.
}


void ProcessMessage(CEvent *p_event, int i_index)
{
    CAction *p_action = p_event->GetAction(i_index);
    TCHAR *p_new_buff = NULL;

    //ok, first of all, modify this sucker if we need to

#ifdef _UNICODE 
    TCHAR *p_buf_to_use =  p_action->GetBufferPointer();
#else

    uni uni_buff(p_action->GetBufferPointer());
    TCHAR *p_buf_to_use =  uni_buff.st_data;

#endif
     if (!p_buf_to_use)
      {
          log_error("Can't process message, it's blank.");
          return; //no message here
      }


     //if a $ is anywhere inside this, let's do our thing on it
     if (wcschr(p_action->GetBufferPointer(), '$') )
     {
       //there is a money symbol in it which means it may need var
       //replacing.  Let's do it.

       //make a buffer for it
         int i_orig_buff_size = wcslen(p_action->GetBufferPointer());
         int i_buff_size = (i_orig_buff_size *2)+C_MAX_URL_SIZE*10; //to be safe
         p_new_buff = new TCHAR[i_buff_size];

#ifdef _UNICODE
         ReplaceStrings(p_new_buff, p_action->GetBufferPointer(), i_buff_size, i_orig_buff_size, p_event);
#else
         ReplaceStrings(p_new_buff, uni_buff.st_data, i_buff_size, i_orig_buff_size, p_event);

#endif
         p_buf_to_use = p_new_buff;
     }
  
    if (p_action->m_b_log)
    {
#ifdef _UNICODE
      LogMsg(p_buf_to_use);
#else
      log_msg(p_buf_to_use);
        
#endif
    }

    if (p_action->m_b_say)
    {
        if (app_glo.GetSpeech())
        {
          //SAPI won't work in a thread, needs the message pump for messaging
          //Let's send it to our main window to play
          //global say will make a copy and send it
           glo.m_voice.Say(p_buf_to_use);
        } else
        {
          //  LogError(L"Microsoft SAPI has been disabled from the options menu, can't use text to speech.");
        }
    }                 
    
     if (p_action->m_b_popup)
    {
      //pop up a window
     //    MessageBox(NULL, p_buf_to_use, _T("Toolfish Alert"),MB_ICONINFORMATION | MB_OK | MB_SYSTEMMODAL);
       CDlgAlert dlg;
       dlg.StoreTextPointerForLater(p_buf_to_use); //WCHAR format
       dlg.SetActionPointer(p_action);
       dlg.DoModal();
     
     }
         
     if (p_action->m_b_macro)
     {
        InjectString(p_buf_to_use, _tcslen(p_buf_to_use));
     }
  
     if (p_action->m_b_email)
     {
         //convert carrage returns
         
         
         SendEmail(p_buf_to_use, p_event, i_index);
     }

     //clear mem if we had allocated it
     SAFE_DELETE_ARRAY(p_new_buff);

}


  void ProcessSound(CEvent *p_event, int i_index)
{
    CAction *p_action = p_event->GetAction(i_index);

#ifdef _UNICODE

    PlaySound(p_action->GetFilename(), NULL, SND_FILENAME | SND_ASYNC);
#else
    PlaySound(uni(p_action->GetFilename()).to_st(), NULL, SND_FILENAME | SND_ASYNC);

#endif
  }
  
  void ProcessRun(CEvent *p_event, int i_index)
  {
      CAction *p_action = p_event->GetAction(i_index);
      
      TCHAR ws_filename[C_MAX_URL_SIZE];
      TCHAR ws_path[C_MAX_URL_SIZE];
  
      get_filename_only(  ws_filename,uni(p_action->GetFilename()).GetAuto());
     _tcscpy(ws_path,  get_path_from_string(uni(p_action->GetFilename()).GetAuto()));

      
      TCHAR *p_temp_parms = NULL;
      //make a buffer for it
      int i_orig_buff_size = wcslen(p_action->GetBufferPointer());
      int i_buff_size = (i_orig_buff_size *2)+C_MAX_URL_SIZE*10; //to be safe
      p_temp_parms = new TCHAR[i_buff_size];
	  memset(p_temp_parms, 0, sizeof(TCHAR) * i_buff_size);
 
      ReplaceStrings(p_temp_parms, uni(p_action->GetBufferPointer()).GetAuto(), i_buff_size, i_orig_buff_size, p_event);
  
      if ( (!p_action->m_b_no_wait) && (file_extension_is(uni(p_action->GetFilename()).GetAuto(), _T(".bat"))) )
      {
          //let's run as a system command, it's fun
           TCHAR *p_line = new TCHAR[wcslen(p_action->GetFilename())+_tcslen(p_temp_parms)+2]; //the one is for a null at the end, 1 is for a space we add
           _tcscpy(p_line, uni(p_action->GetFilename()).GetAuto());
           _tcscat(p_line, _T(" "));
           _tcscat(p_line, p_temp_parms);
           
		   if (!p_event->GetQuietMode())
			LogMsg(_T("RUNNING: %s"), p_line);
           _tsystem(p_line);
           SAFE_DELETE_ARRAY(p_line);
          
      }   else
      {
          
          //run the file
   
          if (
       //       (file_extension_is(uni(p_action->GetFilename()).GetAuto(), _T(".exe"))) ||
              // (file_extension_is(p_action->GetFilename(), L".bat")) ||
         //     (file_extension_is(uni(p_action->GetFilename()).GetAuto(), _T(".com"))) 
         0     
        
         //I don't see a reason why we should even use spawn anymore.
         )
          {

              //executable file method
              
              _flushall();  //says to do this before calling any spawn function, I dunno why
              
              const int i_max_parms = 3;
              
              TCHAR *p_array[i_max_parms];
              
              for (int i=0; i < i_max_parms; i++)
              {
                  p_array[i] = NULL; //pointer to a pointer
              }
              
              p_array[0] = ws_filename;

              //now let's fill this with pointers...
              
              if ( (p_temp_parms) && (p_temp_parms[0] != 0))
              {
                  p_array[1] = p_temp_parms; 
              }
              
              //now point them to various places in our parm string, if applicable
              
              int i_flags = _P_WAIT; //this will wait for it to finish...
             
              if (p_action->m_b_no_wait) i_flags = P_NOWAIT; //this won't wait
              
              
			  if (!p_event->GetQuietMode())
				  LogMsg(_T("RUNNING: %s %s"), p_action->GetFilename(), p_temp_parms);

              
              //CreateProcess
                if (p_action->m_b_no_wait)
                {
                    _tchdir(ws_path);
                    
                } 
              int i_result = _tspawnvp(i_flags, uni(p_action->GetFilename()).GetAuto(), p_array);
              
            if (p_action->m_b_no_wait)
            {
                //switch back to toolfish directory
                switch_to_current_dir();  
            }

              if (i_result == -1)
              {
                  //error occured

                                      
              if (errno == E2BIG) LogError(_T("Error running %s. (Argument list exceeds 1024 bytes)"),uni(p_action->GetFilename()).GetAuto() );
                  else
                      if (errno == ENOEXEC) LogError(_T("Error running %s. (not an executable)"),uni(p_action->GetFilename()).GetAuto() );
                      else
                          if (errno == ENOMEM) LogError(_T("Error running %s. (not enough memory)"),uni(p_action->GetFilename()).GetAuto() );
                          else
                              if (errno == ENOENT) LogError(_T("Error running %s. (bad path or filename)"),uni(p_action->GetFilename()).GetAuto() );
                              
                              else
                                  if (errno == EINVAL) LogError(_T("Error running %s. (mode argument invalid)"),uni(p_action->GetFilename()).GetAuto() );
                                  else
                                  {
                                      LogError(_T("Error running %s. (unknown error with _wspawnvp)"),uni(p_action->GetFilename() ).GetAuto());
                           }
                                  
              }
              
              
          }  else
          {
			  if (!p_event->GetQuietMode())
				  LogMsg(_T("RUNNING: %s %s"), uni(p_action->GetFilename()).GetAuto(), p_temp_parms);
                   
              int result = (int)ShellExecute(NULL,_T("open"),uni(p_action->GetFilename()).GetAuto(),p_temp_parms ,ws_path, SW_SHOWDEFAULT  );
              if (result <= 32)
              {
                  if (result == ERROR_FILE_NOT_FOUND)
                      LogError(_T("Error running %s. (File not found)"), uni(p_action->GetFilename()).GetAuto()); else
                      if (result == ERROR_PATH_NOT_FOUND)
                          LogError(_T("Error running %s. (path not found)"), uni(p_action->GetFilename()).GetAuto()); else
                          
                          if (result == SE_ERR_ASSOCINCOMPLETE)
                              LogError(_T("Error running %s. (file file association incomplete or invalid)"), uni(p_action->GetFilename()).GetAuto()); else
                              if (result == SE_ERR_NOASSOC)
                                  LogError(_T("Error running %s. (no application associated with this file extension)"), uni(p_action->GetFilename()).GetAuto()); else
                                  if (result == SE_ERR_OOM)
                                      LogError(_T("Error running %s. (Out of memory)"), uni(p_action->GetFilename()).GetAuto()); else
                                      
                                  {
                                      
                                      LogError(_T("Error running %s. (unknown reason)"), uni(p_action->GetFilename()).GetAuto());
                                  }
                                  
              }
          }
          
      }
 

                                      
               SAFE_DELETE_ARRAY(p_temp_parms);
      

//switch back to toolfish directory
// switch_to_current_dir();              

      
  }
  
  
  void ProcessWebsite(CEvent *p_event, int i_index)
  {
   CAction *p_action = p_event->GetAction(i_index);

       TCHAR *p_temp_parms = NULL;
      //make a buffer for it
      int i_orig_buff_size = wcslen(p_event->GetAction(i_index)->GetFilename());
      int i_buff_size = (i_orig_buff_size *2)+C_MAX_URL_SIZE*10; //to be safe
      p_temp_parms = new TCHAR[i_buff_size];
    
      ReplaceStrings(p_temp_parms, uni(p_event->GetAction(i_index)->GetFilename()).GetAuto(), i_buff_size, i_orig_buff_size, p_event);
  
      if (p_event->GetAction(i_index)->GetOpenInSameWindow())
      {
          
          winall_create_url_file_full(uni(p_temp_parms).to_st());
          int result = (int)ShellExecute(NULL,_T("open"),_T("temp.url"), NULL,NULL, SW_MAXIMIZE);
#ifdef _UNICODE
          ProcessError(app_glo.GetHWND(), result, _T("temp.url"),p_event->GetAction(i_index)->GetFilename()); //show an error message if we couldn't open this
#else
          ProcessError(app_glo.GetHWND(), result, _T("temp.url"),uni(p_event->GetAction(i_index)->GetFilename()).to_st()); //show an error message if we couldn't open this
          
#endif
       
          


      } else
      {
          
         LaunchURL(p_temp_parms);
      }
 
      
      SAFE_DELETE_ARRAY(p_temp_parms);


  }
  
  void ProcessVolume(CEvent *p_event, int i_index)
  {
      
      if (p_event->GetAction(i_index)->GetTTS())
      {
          DeactivateMute();
          app_glo.ResetMuteTimer();
      }
      
      ULONG mmin,mmax;
      LONG u_cur_vol = GetMasterVolume(mmin,mmax);
      long lLeftVol = ( ((long)mmax - (long)mmin) / 10) * p_event->GetAction(i_index)->GetVolume();
      if (mmin < 0)
      {
          //compensate for a negetive based minimum value
          lLeftVol += mmin;
      }
      
      
      if (app_glo.GetMuted())
      {
          //ok, it's muted now so let's set the vol for when it gets
          //unmuted instead of the real system vol
          app_glo.SetUnmuteVol(lLeftVol);
      }   else
      {
          
          if (p_event->GetAction(i_index)->GetFade())
          {
              //smoothly fade the volume up.  Since this is on a separate thread, we can do it right
              //here.
              ULONG ul_step_size = ULONG(float(mmax) * 0.01f);
              int i_emergency_counter = 0;
              while (lLeftVol != u_cur_vol)
              {
                set_long_with_target(&u_cur_vol, lLeftVol, ul_step_size);
                if (i_emergency_counter++ == 1000)
                {
                    //this should never happen.  Maybe this soundcard has min/max values our math
                    //is incompatible with, abort.
                    u_cur_vol = lLeftVol;
                    assert(0);
                }
                SetMasterVolume(u_cur_vol);
                Sleep(50);
                //log_msg("Stepped to %u.", u_cur_vol);
               }

          } else
          {
               
              //set the real volume right now, without fading
              SetMasterVolume(lLeftVol);
          }
          
      }
      
  }
  
  
  UINT ProcessEventsThread(LPVOID pVoid)
{
   
     app_glo.IncActiveThreadCount();

    CEvent *p_event = (CEvent*) pVoid; //remember this

  //  log_msg("Thread started.");

    //process each event
    for (int i=0; i < p_event->GetActionCount(); i++)
    {
        if (p_event->GetAction(i))
        {
            //trigger is active.  But what is it?
            if (p_event->GetAction(i)->GetActionType() == C_ACTION_MESSAGE)
            {
                ProcessMessage(p_event, i);
            }
             if (p_event->GetAction(i)->GetActionType() == C_ACTION_ATOMIC)
            {
                ProcessAtomicClock(p_event, i);
            }
            
             if (p_event->GetAction(i)->GetActionType() == C_ACTION_SOUND)
            {
                ProcessSound(p_event, i);
            }
            if (p_event->GetAction(i)->GetActionType() == C_ACTION_WEBSITE)
            {
                ProcessWebsite(p_event, i);
            }
                if (p_event->GetAction(i)->GetActionType() == C_ACTION_RUN)
            {
                ProcessRun(p_event, i);
            }
            if (p_event->GetAction(i)->GetActionType() == C_ACTION_COMPARE)
            {
               if (!ProcessCompare(p_event, i))
               {
                   //webpage not updated, don't run any more actions on this event
                   break;
               }
            }
           if (p_event->GetAction(i)->GetActionType() == C_ACTION_MONITOR)
            {
               if (!ProcessMonitor(p_event, i))
               {
                   //site is fine, don't run any more actions on this event
                   
                   //unless... this is a reverse monitor.
                   if (!p_event->GetAction(i)->GetMonitorReverse())
                   {
                       
                       break;
                   }
               }
            }
                if (p_event->GetAction(i)->GetActionType() == C_ACTION_STEALTH)
            {
                SendMessage(app_glo.GetHWND(), WM_TOGGLE_STEALTH, 0,0);
            }
                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_EMAIL)
            {
                  if (!ProcessEmail(p_event, i))
                  {
                      //no new email, don't run rest of the events
                      break;
                  }

            }
                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_TOGGLE)
                     {
                         
                             SendMessage(app_glo.GetHWND(), WM_TOGGLE_TOOLFISH, 0,0);
                          
                     }

                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_TTS)
                     {
                         
                             app_glo.ActivateSpeech(p_event->GetAction(i)->GetTTS());

                     }

                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_CHANGE_REZ)
                     {

                         LogMsg("Resetting screen resolution.  This fixes the black screen issue with some NVidia cards.");
                        // ChangeRez();
                         DEVMODE devmode;
                         EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
                         devmode.dmSize = sizeof(DEVMODE);
                         devmode.dmFields = DM_PELSWIDTH || DM_PELSHEIGHT;

                         long result = ChangeDisplaySettings(&devmode, CDS_RESET);

                         if (result != DISP_CHANGE_SUCCESSFUL)
                         {
                             LogMsg("Error:  Couldn't reset graphic mode.  (%d)", result);
                         }

                     }
                      if (p_event->GetAction(i)->GetActionType() == C_ACTION_SMARTMUTE)
                     {
                         
                           glo.m_b_smart_mute = p_event->GetAction(i)->GetTTS();
                          
                          if (!glo.m_b_smart_mute)
                          {
                              //if we're muted right now we need to jump start it
                              DeactivateMute();
                          }

                     }
                      if (p_event->GetAction(i)->GetActionType() == C_ACTION_VOLUME)
                      {
                          
                          ProcessVolume(p_event, i);
                      }
					  if (p_event->GetAction(i)->GetActionType() == C_ACTION_MIC_VOLUME)
					  {

						  AdjustMic(uni(p_event->GetAction(i)->GetFilename()).to_st(), p_event->GetAction(i)->GetVolume()/100.0f);
						  //ProcessVolume(p_event, i);
					  }
                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_SEND)
                      {
                          
                          SendCommandToApp(p_event, i);
                      }
                    if (p_event->GetAction(i)->GetActionType() == C_ACTION_DELAY)
                      {
                          
                        Sleep(p_event->GetAction(i)->GetDelay());

                      }
                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_REBOOT)
                      {
                          
                          ProcessReboot(p_event, i);
                      }
                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_LEET)
                      {
                        //toggle leet-type  
                       glo.m_b_key_overlay_active = !glo.m_b_key_overlay_active;
                       SetupKeyboardOverlay();
                       }
                     if (p_event->GetAction(i)->GetActionType() == C_ACTION_SOUND_OUTPUT)
                      {
                          // Set the sound output device by substring match
                          uni deviceMatch(p_event->GetAction(i)->GetFilename());
                          SetDefaultOutputDevice(deviceMatch.to_st());
                      }
                     
     
       }                                                                         
    }
        app_glo.DecActiveThreadCount();
       //save the event
        //modidy time
        p_event->SetLastRunTimeToNow();

        app_glo.GetEventList()->SaveEvent(p_event->GetIndex());

        //delete if required
        if (p_event->GetAutoDeletePermission())
        {
            p_event->SetAutoDeletePermission(false);

            if (p_event->GetAutoDeleteAfterRunning())
            {
                //instructed to delete this after being run, now is an ok time to do it.
                app_glo.GetEventList()->DeleteEvent(p_event->GetIndex());
            }

        }


  //  log_msg("Thread ended.");
    return 0;
}


  void ProcessMute()
  {
      if (!app_glo.GetMuted())
      {
          //should we mute?
        if (!glo.m_b_smart_mute) return; //function not enabled

        //has enough time passed?
        if (app_glo.TimeToMute())
        {
            ActivateMute();
        }
 
      }  else
      {
             //clean up misc mouse movement
         app_glo.CleanUpMouseMovement(); //a second passed
      }
  }

  void ProcessTimerEvents()
  {
      ProcessMute();
#ifdef _USES_PROTECTION
      app_glo.SecurityCheck();
#endif
      if (app_glo.GetPaused()) return;
      
      CTime t_now = CTime::GetCurrentTime();
      
      t_now.GetCurrentTime();
      
      //   log_msg("Time is %s.", (LPCSTR)t_now.Format("%A, %B %d, %Y"));
      
      
      //ok, we now have the current time.  Should an event be run?
      CEvent *p_event = NULL;
      for (int i_event=0; i_event < app_glo.GetEventList()->GetCount(); i_event++)
      {
          if (app_glo.GetEventList()->GetEvent(i_event))
          {
              p_event = app_glo.GetEventList()->GetEvent(i_event);
              
              if (!p_event->GetEnabled()) continue; //event disabled
                  
              //event isn't null so I guess it's active
              bool b_run_it = true;
              bool b_delete_after = false;
              if ( (p_event->GetTriggerCount() == 0 ) || !p_event->GetTrigger(0))
              {
                  //this may be an empty trigger, cycle through and make sure
                  b_run_it = false; //disable event
                  for (int l=0; l < p_event->GetTriggerCount(); l++)
                  {
                      if (p_event->GetTrigger(l))
                      {
                          //ha, it actually had a valid trigger, turn it back on
                          b_run_it = true;
                      }
                  
                  }
                  
                  //added in 1.70, ignore things with no triggers
              }
              for (int i=0; i < p_event->GetTriggerCount(); i++)
              {
                  if (p_event->GetTrigger(i))
                  {
                      
                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_HOTKEY)
                      {
                          b_run_it = false;
                          //if it has a hotkey it should never be run on a timed event.  should it?
                          break; 
                      }
                      
                      
                      //valid trigger.  deal with it
                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_TIME)
                      {
                          
                          CTime p_t_trigger_time = p_event->GetTrigger(i)->GetTime(); //copy the time
                          
                          if (p_event->GetTrigger(i)->TimeTriggered(&t_now, p_event))
                          {
                              //do an emergency check, fail the check if there is a day filter that invalidates
                              //this
                              for (int k=i; k < p_event->GetTriggerCount(); k++)
                              {
                                  if (p_event->GetTrigger(k))
                                  {
                                      if (p_event->GetTrigger(k)->GetTriggerType() == C_TRIGGER_DAY_FILTER)
                                      {
                                          if (!p_event->GetTrigger(k)->m_b_a_day_active[p_t_trigger_time.GetDayOfWeek()-1])
                                          {
                                              b_run_it = false; //today isn't approved to run.
                                              break; 
                                          }
                                      }
                                      
                                      
                                  }
                              }
                              
                              if (!p_event->GetTrigger(i)->GetRescheduleActive())
                              {
                                  //it was a time trigger and it wasn't set to reschedule
                                  //I guess we should disable the whole event?
                                  b_delete_after = true;
                              }
                              
                              if (!b_run_it) break;
                              
                          }   else
                          {
                              b_run_it = false; //not time yet.
                              break;
                          }
                          
                      }
                      
                      
                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_TIME_RANGE)
                      {
                          //if it has a hotkey it should never be run on a timed event.  should it?
                          
                          CTime time = CTime(2002,1,1,CTime::GetCurrentTime().GetHour(),
                              CTime::GetCurrentTime().GetMinute(),CTime::GetCurrentTime().GetSecond());
                          
                          if (!p_event->GetTrigger(i)->TimeIsBetweenRange(time))
                          {
                              
                              b_run_it = false; //not time yet.
                              break; 
                          }
                        
                      }
                      

                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_DAY_FILTER)
                      {
                          //if it has a hotkey it should never be run on a timed event.  should it?
                          
                          CTime time_now = CTime::GetCurrentTime();
                          if (!p_event->GetTrigger(i)->m_b_a_day_active[time_now.GetDayOfWeek()-1])
                          {
                              b_run_it = false; //today isn't approved to run.
                              break; 
                          }
                          
                      }
                      
                      

                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_INACTIVITY)
                      {
                          if (p_event->GetTrigger(i)->m_b_check_for_activity)
                          {
                              if (app_glo.GetSecondsInactive() > p_event->GetTrigger(i)->GetRescheduleTimeInSeconds())
                              {
                                  //they've touched the computer too long ago for this trigger to work
                                  b_run_it = false; //not time yet.
                                  break; 
                                  
                              }
                          } else
                          {
                              if (app_glo.GetSecondsInactive() < p_event->GetTrigger(i)->GetRescheduleTimeInSeconds())
                              {
                                  //they've touched the computer too recently
                                  b_run_it = false; //not time yet.
                                  break; 
                                  
                              }
                              
                              
                          }
                          
                          
                      }
                  
                       if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_PROCESS)
                      {
                         if (!ProcessProcessTrigger(p_event, i))
                          {
                              b_run_it = false; //now isn't approved to run.
                              break; 
                          }
                          
                      }
                  


                  
                  
                  } 
                  
                  
              }
              
              //all triggers processed.  Should we run it?
              if (!app_glo.m_b_tampered)
              if (b_run_it)
              {
                   if (b_delete_after)
                  {
                      p_event->SetEnabled(false);
                      
                  }
                  p_event->SetAutoDeletePermission(true); //let it know it can delete the event if it wants to later, this is
                  //run by the timer and not a test run.
                  
                  //let's start it's thread
                  RunEvent(p_event);
                  

              }
              
              
          }
      }
   
  }

  void RunEvent(CEvent *p_event)
  {
      if (!p_event->GetQuietMode())      
      LogMsg(_T("Triggered %s."), p_event->GetSmartNameLong());
      AfxBeginThread(ProcessEventsThread, (LPVOID)p_event);
  }

void BuildHotKeyInfo()
{

        ClearKeys();
        
        //check every hotkey in every action and add it's info
      CEvent *p_event = NULL;
      for (int i_event=0; i_event < app_glo.GetEventList()->GetCount(); i_event++)
      {
          if (app_glo.GetEventList()->GetEvent(i_event))
          {
              p_event = app_glo.GetEventList()->GetEvent(i_event);
              
              if (!p_event->GetEnabled()) continue; //event disabled
              
               for (int i=0; i < p_event->GetTriggerCount(); i++)
              {
                   CTrigger *p_trigger;
                   if (p_trigger = p_event->GetTrigger(i))
                  {
                      
                      if (p_trigger->GetTriggerType() == C_TRIGGER_HOTKEY)
                      {
                          //found a hotkey, let's add its info
                          AddKey(p_trigger->m_vk_key,
                            p_trigger->m_b_ctrl,
                            p_trigger->m_b_alt,
                            p_trigger->m_b_shift,
                            p_trigger->m_b_pass_through,
                            0 /*replace key, not used yet*/
                            );
                          
                          break; 
                      }
                      
                  }
               }
          }
      }

       
}


void ProcessHotkeyEvents(char vk)
{
    bool b_shift = false;
    bool b_ctrl = false;
    bool b_alt = false;
    
    if (GetAsyncKeyState(VK_SHIFT)) b_shift = true;
    if (GetAsyncKeyState(VK_CONTROL)) b_ctrl = true;
    if (GetAsyncKeyState(VK_MENU)) b_alt = true;


    //go through the triggers

  if (app_glo.GetPaused()) return;
    
    CTime t_now = CTime::GetCurrentTime();

   t_now.GetCurrentTime();

//   log_msg("Time is %s.", (LPCSTR)t_now.Format("%A, %B %d, %Y"));
  

   //ok, we now have the current time.  Should an event be run?
   CEvent *p_event = NULL;
   for (int i_event=0; i_event < app_glo.GetEventList()->GetCount(); i_event++)
   {
       if (app_glo.GetEventList()->GetEvent(i_event))
       {
           p_event = app_glo.GetEventList()->GetEvent(i_event);

           if (!p_event->GetEnabled()) continue; //event disabled
           //event isn't null so I guess it's active
           bool b_run_it = true;
           bool b_delete_after = false;
          
           
           //if a hotkey event does not exist, don't process it at all.
                
                  bool b_continue = false;
                  for (int l=0; l < p_event->GetTriggerCount(); l++)
                  {
                      if (p_event->GetTrigger(l)) //make sure it's valid
                      if (p_event->GetTrigger(l)->GetTriggerType() == C_TRIGGER_HOTKEY)
                      {
                          b_continue = true;
                       
                          
                      }
                  
                  }
                  

                  //added in 1.70, ignore things with no triggers
            
             if (b_continue)
             {
             
           for (int i=0; i < p_event->GetTriggerCount(); i++)
           {
               if (p_event->GetTrigger(i))
               {
                   //valid trigger.  deal with it
                   if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_TIME)
                   {
                       b_run_it = false; //not time yet.
                   }
              
                  
                     if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_PROCESS)
                      {
                         if (!ProcessProcessTrigger(p_event, i))
                          {
                              b_run_it = false; //now isn't approved to run.
                              break; 
                          }
                          
                      }

                   
                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_TIME_RANGE)
                      {
                          //if it has a hotkey it should never be run on a timed event.  should it?
                          
                          CTime time = CTime(2002,1,1,CTime::GetCurrentTime().GetHour(),
                              CTime::GetCurrentTime().GetMinute(),CTime::GetCurrentTime().GetSecond());
                          
                          if (!p_event->GetTrigger(i)->TimeIsBetweenRange(time))
                          {
                              
                              b_run_it = false; //not time yet.
                              break; 
                          }
                        
                      }
                      

                      if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_DAY_FILTER)
                      {
                          //if it has a hotkey it should never be run on a timed event.  should it?
                          
                          CTime time_now = CTime::GetCurrentTime();
                          if (!p_event->GetTrigger(i)->m_b_a_day_active[time_now.GetDayOfWeek()-1])
                          {
                              b_run_it = false; //today isn't approved to run.
                              break; 
                          }
                          
                      }
                      
                      

                   
                   if (p_event->GetTrigger(i)->GetTriggerType() == C_TRIGGER_HOTKEY)
                   {
                      
                       
                       if (p_event->GetTrigger(i)->HotkeyPressed(vk, b_shift, b_ctrl, b_alt))
                       {

                       }   else
                       {
                       
                       b_run_it = false; //not time yet.
                       }
                   }
               }
               
               
               
           }
           
           //all triggers processed.  Should we run it?
           if (b_run_it)
           {
               if (!p_event->GetQuietMode())      
               LogMsg(_T("Triggered %s."), p_event->GetSmartName());
                
                if (b_delete_after)
                {
                    p_event->SetEnabled(false);
           
                }
                    //save the event
                    app_glo.GetEventList()->SaveEvent(p_event->GetIndex());


                //let's start it's thread
              AfxBeginThread(ProcessEventsThread, (LPVOID)p_event);
    
           }
             }

       }
   }
  

}
