/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



/*

General purpose timing device.  Yes, it has mixed styles, because
I switched over the years but have a lot of legacy code...


Use like this:

CTimer timer;

  timer.set_interval(1000);

  while (1)
  {
    if (timer.interval_reached())
    {
       Print("Holy snickey, one second passed!");
    }
  }

*/


#pragma once
//timing system in 1000's of a second
#include "all.h"

const int C_INTERVAL_DISABLED = -1; //by default no timer is running
class CTimer
{
public:
  
    CTimer(); //constructor
    ~CTimer(); //deconstructor
    
    void set_interval(int i_new_interval);
    bool interval_reached();
    void reset(); //set everything to 0's, disabled interval.
    int get_elapsed_milliseconds();
    void stop_time();
    void ResetInterval(); //start the interal over again from now regardless
    void TriggerInterval(); //it will trigger yes the next time interval_reached is called
    bool IsIntervalActive(){return (i_interval != C_INTERVAL_DISABLED);};
    int GetInterval(){return i_interval;}
    int GetTimerStart(){return i_timer_start;}
    int get_elapsed_milliseconds_force()
    {
        return (GetTickCount() -i_timer_start);
  }
  
  int CTimer::get_elapsed_seconds_force()
  {
      int i_elapsed = timeGetTime() -i_timer_start;
      if (i_elapsed == 0) return 0; //avoi devide by 0 error
      return (i_elapsed / 1000); //convert to seconds
  }
  
protected:

  DWORD i_timer_start;
  DWORD i_creation_time;
  DWORD i_interval;
  DWORD i_stop_time;

};