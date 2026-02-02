#include "AtomicClock.h"
 
#include "..\Shared\all.h"
#include "..\Shared\CTextParse.h"
#include "..\Shared\winsock\winsock_util.h"
#include "..\Shared\winsock\RawSocket.h"
#include "..\Shared\uni.h"
#include "..\Shared\mfc_all.h"


bool ProcessAtomicClock(CEvent *p_event, int i_index)
{                                            
    CAction *p_action = p_event->GetAction(i_index);
    
    CWizReadWriteSocket socket;
#ifdef _UNICODE
    if(!socket.Connect(p_action->GetEmailServer(), 13))
#else
    if(!socket.Connect(uni(p_action->GetEmailServer()).st_data, 13))
#endif        
    {
        log_msg("Unable to connect with Atomic Clock server %s.",
            uni(p_action->GetEmailServer()).st_data);
        return false;
    }
    
    char st_buff[513];
    memset(st_buff, 0, 513);
    CTextParse parse;
    int i_read = 0;    
    
    while ( (i_read = socket.Read((char*)&st_buff, 512)) > 0)
    {
        //log_msg(st_buff);
        parse.AddText(st_buff, i_read);
    }
    
    if (parse.GetBuffByteSize() < 5)
    {
        log_msg("Unable to read from Atomic Clock server %s. (Maybe already connected?)",
            uni(p_action->GetEmailServer()).st_data);
          return false;
    }

    //now parse the string and figure out what time it is.
    SYSTEMTIME time_system;
    char *p_line;
    char st_temp[1024];
    char st_num[512];
    
    while (parse.get_next_line(&p_line))
    {
        
        if (atol(parse.get_word(1, ' ')) > 52649)
        {
            //found valid date.
            if (!parse.GetBufferAtWord("UTC"))
            {
            log_msg("Atomic Clock data on server %s does not appear to be in UTC format, aborting time change.",
            uni(p_action->GetEmailServer()).st_data);
            return false;
       }
            
            strcpy(st_temp, parse.get_word(2, ' '));
            //YEAR            
            seperate_string(st_temp, 1, '-', (char*)&st_num);
            time_system.wYear = atol(st_num)+2000;
            //MONTH            
            seperate_string(st_temp, 2, '-', (char*)&st_num);
            time_system.wMonth = WORD(atol(st_num));
            //DAY
            seperate_string(st_temp, 3, '-', (char*)&st_num);
            time_system.wDay = WORD(atol(st_num));
          
            strcpy(st_temp, parse.get_word(3, ' '));
            //HOUR
             seperate_string(st_temp, 1, ':', (char*)&st_num);
            time_system.wHour = WORD(atol(st_num));
            //MINUTE
             seperate_string(st_temp, 2, ':', (char*)&st_num);
            time_system.wMinute = WORD(atol(st_num));
            //SECOND
             seperate_string(st_temp, 3, ':', (char*)&st_num);
            time_system.wSecond = WORD(atol(st_num));
         
            //set some stuff we don't know
            time_system.wMilliseconds = 0;
            time_system.wDayOfWeek = 0; //supposed to be ignored, but I'll set it anyway.

            //let's validate all the data and make sure none of it is crazy
            if (
                (!in_range(time_system.wYear, 2003, 4003))
                ||   (!in_range(time_system.wDay, 0, 32))
                ||   (!in_range(time_system.wHour, 0, 25))
                ||   (!in_range(time_system.wMonth, 1, 13))
                ||   (!in_range(time_system.wMinute, 0, 61))
                ||   (!in_range(time_system.wSecond, 0, 61))
                )
            {
                    log_msg("Atomic Clock data on server %s does not appear to be in a valid format, aborting time change.",
            uni(p_action->GetEmailServer()).st_data);
            return false;
   
            }
  


            
            //get difference in times


            SYSTEMTIME current_system_time;
            GetSystemTime(&current_system_time);

            //Actually set the clock.
            CTime old_time = CTime(current_system_time);
            CTime new_time = CTime(time_system);

            
            //great, now let's compare them.
            CTimeSpan difference = new_time - old_time;
           
            bool b_set_time = true;

            if (difference.GetHours() != 0 || difference.GetDays() != 0)
            {
                
                LogMsg(_T("Clock time adjusted by %d hour(s), %d minute(s) and %d second(s) to match the atomic clock."),
                    difference.GetHours(), difference.GetMinutes(), difference.GetSeconds());
            } else
            if (difference.GetMinutes() != 0)
            {
                  
                    LogMsg(_T("Clock time adjusted by %d minute(s) and %d second(s) to match the atomic clock."),
                        difference.GetMinutes(), difference.GetSeconds());
                    
                    
            } else
              if (difference.GetSeconds() != 0)
            {
                  
                  LogMsg(_T("Clock time adjusted by %d second(s) to match the atomic clock."),
                 difference.GetSeconds());
                                                                 
            }   else
              {
                //I guess no change was made
                     LogMsg(_T("Local time perfectly matches the atomic clock, no time adjustment made."));
                   b_set_time = false; 
            }
                
                
            if (b_set_time)    
            {
                if (!SetSystemTime(&time_system))
                {
                    log_msg("Error setting system time.");
                }                
            }

        }
        
    }
    
    
    
  //52650 03-01-11 07:43:19 00 0 0 351.8 UTC(NIST) *


    return true; //success
}
