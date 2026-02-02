/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


#include "std_all.h"

void winall_create_url_file(char url[])
{
    //create temp.url
    
    //delete old file if applicable
    unlink("temp.url");
    char st_file[30], st_text[1024]; 
    strcpy(st_file, "temp.url");
    sprintf(st_text, "[InternetShortcut]\n");
    add_text(st_text, st_file);
    sprintf(st_text, "URL=http://%s\n",url);
    add_text(st_text, st_file);
    
}

 void winall_create_url_file_full(char url[])
{
    //create temp.url
    
    //delete old file if applicable
    unlink("temp.url");
    char st_file[30], st_text[1024]; 
    strcpy(st_file, "temp.url");
    sprintf(st_text, "[InternetShortcut]\n");
    add_text(st_text, st_file);
    sprintf(st_text, "URL=%s\n",url);
    add_text(st_text, st_file);
    
}


 
void create_url_file(char url[255])
{
    //create temp.url
    
    //delete old file if applicable
    unlink("temp.url");
    char st_file[30], st_text[255]; 
    strcpy(st_file, "temp.url");
    sprintf(st_text, "[InternetShortcut]\n");
    add_text(st_text, st_file);
    sprintf(st_text, "URL=http://%s\n",url);
    add_text(st_text, st_file);
    
}



//returns true if this is running on 95, 98 or ME

bool WindowsIs9xVersion() 
{ 
    OSVERSIONINFOEX winfo; 
    
    ZeroMemory(&winfo, sizeof(OSVERSIONINFOEX)); 
    winfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX); 
    
    if (GetVersionEx((OSVERSIONINFO *)&winfo) == 0) 
    { 
        //Get Windows version failed 
        return FALSE; 
    } 
    
   
        if (winfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) 
        { 
            if(winfo.dwMinorVersion < 10) 
            {
                //WinVer = W95; //Windows 95 
                return true;
            }
            else if (winfo.dwMinorVersion < 90) 
            {
             //   WinVer = W98; //Windows 98 
            return true;
            } 
            else 
            {
            
               // WinVer = WME; //Windows ME 
            return true;
            }
        } 
   
    
    return false; 
} 




//yes I will templatize these.. tomorrow

bool force_range(int * i_original, int i_min, int i_max)
{
 if (in_range(*i_original, i_min, i_max)) return false;
 if (*i_original < i_min) *i_original = i_min;
 if (*i_original > i_max) *i_original = i_max;

 return false;

}

bool lforce_range(long * l_original, long l_min, long l_max)
{
 if (in_range(*l_original, l_min, l_max)) return false;
 if (*l_original < l_min) *l_original = l_min;
 if (*l_original > l_max) *l_original = l_max;

 return false;

}


float lerp_float(float f_origin, float f_target, float f_percent)
{
  return (f_origin - ((f_origin-f_target)*f_percent));

}

double lerp_double(double origin, double target, double percent)
{
    return (origin - ((origin-target)*percent));
}


bool fforce_range(float * i_original, float i_min, float i_max)
{
 if (in_range_float(*i_original, i_min, i_max)) return false;
if (*i_original < i_min) *i_original = i_min;
 if (*i_original > i_max) *i_original = i_max;

 return false; //changed number
}


//this let's you apply a number to a number to make it closer to a target
//it will not go pass the target number.
void set_float_with_target(float *p_float, float f_target, float f_friction)
{
  if (*p_float != f_target)
  {
    if (*p_float > f_target)
	{
	  *p_float -= f_friction;
	  if (*p_float < f_target) *p_float = f_target;
	} else
	{
	 *p_float += f_friction;
	 if (*p_float > f_target) *p_float = f_target;
	}
  }
}


void set_double_with_target(double *p_double, double f_target, double f_friction)
{
    if (*p_double != f_target)
    {
        if (*p_double > f_target)
        {
            *p_double -= f_friction;
            if (*p_double < f_target) *p_double = f_target;
        } else
        {
            *p_double += f_friction;
            if (*p_double > f_target) *p_double = f_target;
        }
    }
}


//note to self:  templatize this later or something
void set_long_with_target(long *p_long, long f_target, long f_friction)
{
  if (*p_long != f_target)
  {
    if (*p_long > f_target)
	{
	  *p_long -= f_friction;
	  if (*p_long < f_target) *p_long = f_target;
	} else
	{
	 *p_long += f_friction;
	 if (*p_long > f_target) *p_long = f_target;
	}
  }
}





//example, to get how many days have passed since Feb 1st, 2002 you would do this:
//i_days = GetDaysSinceDate(2, 1, 2002);

int GetDaysSinceDate(int i_month,int i_day, int i_year)
{

	time_t ltime;
	
	time( &ltime );
	
	// { 0, 0, 0, 25, 4, 100, 0 } expires on 00:00:00 25/5/2000 note the month!
	
	tm expire = { 0, 0, 0, i_day, i_month-1, i_year-1900, 0 };	 //Month is 0-11 btw
	tm today = *localtime( &ltime );
	
	long time_now = (long)today.tm_mday + (long)today.tm_mon * 30 + today.tm_year*365;
	long time_exp = (long)expire.tm_mday +(long)expire.tm_mon * 30 + expire.tm_year * 365;
	long time_passed = time_now - time_exp;
	//now let's convert it back to days
	if (time_passed == 0) return 0; //avoid device by 0
	return (time_passed);

}


int GetDaysSinceDate(time_t from_time)
{

	time_t ltime;
	
	time( &ltime );
	
	// { 0, 0, 0, 25, 4, 100, 0 } expires on 00:00:00 25/5/2000 note the month!
	
	tm expire = *localtime( &from_time );
	tm today = *localtime( &ltime );
	
	long time_now = (long)today.tm_mday + (long)today.tm_mon * 30 + today.tm_year*365;
	long time_exp = (long)expire.tm_mday +(long)expire.tm_mon * 30 + expire.tm_year * 365;
	
	long time_passed = time_now - time_exp;

	//now let's convert it back to days
	if (time_passed == 0) return 0; //avoid device by 0

	
	return (time_passed);

}


#ifndef _UNICODE
char *show_date_month_and_day(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    static char st_date[255];
    int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        "MMM d",
        st_date,
        255);
    if (result == 0)
    {
        LogError("Get date function failed.\n");
        return NULL;
    }
    return st_date;
}

 
char *show_army_time(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    static char st_date[255];
    int result = GetTimeFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        "HH':'mm'",
        st_date,
        255);
    if (result == 0)
    {
        LogError("Get date function failed.\n");
        return NULL;
    }
    
    return st_date;
}


char *show_date(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
     
    static char st_date[255];
    int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        "dddd, MMMM dd yyyy",
        st_date,
        255);
     
    if (result == 0)
    {
        LogError("Get date function failed.\n");
        return NULL;
    }
    return st_date;
}




char *show_time(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    static char st_date[255];
    int result = GetTimeFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        "h':'mm' 'tt",
        st_date,
        255);
    if (result == 0)
    {
        LogError("Get date function failed.\n");
        return NULL;
    }
    
    return st_date;
}

 
char *show_small_date(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    static char st_date[255];
    int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        "MM-dd",
        st_date,
        255);
    if (result == 0)
    {
        LogError("Get date function failed.\n");
        return NULL;
    }
    return st_date;
}


#else


TCHAR *show_date(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    
    static TCHAR st_date[255];
    int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        L"dddd, MMMM dd yyyy",
        st_date,
        255);
     
    if (result == 0)
    {
        log_error("Get date function failed.\n");
        return NULL;
    }
    return st_date;
}

TCHAR *show_time(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    static TCHAR st_date[255];
    int result = GetTimeFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        L"h':'mm' 'tt",
        st_date,
        255);
    if (result == 0)
    {
        log_error("Get date function failed.\n");
        return NULL;
    }
    
    return st_date;
}
TCHAR *show_small_date(void)
{
    
    SYSTEMTIME lp_system;
    GetLocalTime(&lp_system);
    
    static TCHAR st_date[255];
    int result = GetDateFormat(LOCALE_USER_DEFAULT, //locale
        NULL, //flags
        &lp_system, //system time
        L"MM-dd",
        st_date,
        255);
     
    if (result == 0)
    {
        log_error("Get date function failed.\n");
        return NULL;
    }
    return st_date;
}       
   


#endif

TCHAR * ShowTimePeriod(int i_total_seconds)
{
    
    
    TCHAR t_buffer[1024];
    TCHAR t_temp[512];
    
    if (i_total_seconds == 0)
    {
        _tcscpy(t_buffer, _T("no seconds"));
    }   else
    {
        
        
        
        
        //break it down into time segments
        
        int i_months = (((i_total_seconds/60)/60)/24)/30;
        int i_months_in_seconds = i_months * 30*24*60*60;
        int i_weeks =  ((((i_total_seconds-i_months_in_seconds)/60)/60)/24)/7;
        int i_weeks_in_seconds = i_weeks * 60*60*24*7;
        
        int i_days = i_total_seconds-(i_weeks_in_seconds+i_months_in_seconds);
        i_days = ((i_days / 60)/60)/24;
        int i_days_in_seconds =  i_days*60*60*24;
        
        int i_hours = i_total_seconds- (i_weeks_in_seconds + i_months_in_seconds + i_days_in_seconds);
        i_hours = (i_hours / 60)/60;
        
        int i_hours_in_seconds = i_hours*60*60;
        
        int i_minutes = (i_total_seconds - (i_weeks_in_seconds+i_months_in_seconds+i_days_in_seconds+i_hours_in_seconds));
        i_minutes = i_minutes / 60;
        int i_seconds = i_total_seconds - (i_weeks_in_seconds+i_months_in_seconds+i_days_in_seconds+i_hours_in_seconds+(i_minutes*60));
        
        
        //build the text string
        t_buffer[0] = 0; //clear it out
        
        if (i_months> 0)
        {
            _stprintf(t_temp,  _T("%d month%s "),i_months, &_T("s")[(i_months == 1)]);
            _tcscat(t_buffer,t_temp);
        }
        
        if (i_weeks> 0)
        {
            _stprintf(t_temp,  _T("%d week%s "),i_weeks, &_T("s")[i_weeks == 1]);
            _tcscat(t_buffer,t_temp);
        }
        if (i_days > 0)
        {
            _stprintf(t_temp,  _T("%d day%s "),i_days, &_T("s")[i_days == 1]);
            _tcscat(t_buffer,t_temp);
        }

        if (i_hours > 0)
        {
            _stprintf(t_temp,  _T("%d hour%s "),i_hours, &_T("s")[i_hours == 1]);
            _tcscat(t_buffer,t_temp);
        }
 
        if (i_minutes > 0)
        {
            _stprintf(t_temp,  _T("%d minute%s "),i_minutes, &_T("s")[i_minutes == 1]);
            _tcscat(t_buffer,t_temp);
        }

        if (i_seconds > 0)
        {
            _stprintf(t_temp,  _T("%d second%s"),i_seconds, &_T("s")[i_seconds == 1]);
            _tcscat(t_buffer,t_temp);
        }

        //remove the ending space if there is one

        strip_ending_spaces(t_buffer);

    }
    
    TCHAR *p_return = new TCHAR[_tcslen(t_buffer)+1];
    _tcscpy(p_return, t_buffer);
    return p_return; //you must delete this yourself!
}

