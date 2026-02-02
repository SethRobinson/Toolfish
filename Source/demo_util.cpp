#include "stdafx.h"
#include "demo_util.h"
#include "..\Shared\CProtection.h"
#include "..\Shared\registry.h"   


int InitTrialStuff()
{

    char st_temp[256];
    cls_pro.MakeUniqueStringFromSystemInfo(st_temp, 5);

    //first we need to decide if this is the first time they've run it or not
     CRegistry reg;
	 CString sRegPath;
     sRegPath.Format(_T("Software\\%s"), uni(st_temp).us_data);	

     bool b_first_time = false;
     CTime time_loaded = 0;

 		if (!reg.VerifyKey(HKEY_LOCAL_MACHINE, sRegPath))
		{
			b_first_time = true; 
		}	


        if (b_first_time)
        {

            //if too many data file exists it means they probably already had this installed
            //and somehow cheated the system.  

            
            /*
            app_glo.SetTrialDaysLeft(0); //trial period is OVER, cheaters!
            return 0;
             */


            //it's a good install, write the time of the starting period.
           	reg.CreateKey(HKEY_LOCAL_MACHINE, sRegPath);
            
            CTime time_now = CTime::GetCurrentTime();
            reg.WriteDWORD(uni(st_temp).GetAuto(), *(const unsigned long*)&time_now); 
            time_loaded = time_now;

        } else
        {

            //read the key
        reg.Open(HKEY_LOCAL_MACHINE, sRegPath);
        reg.ReadDWORD(uni(st_temp).GetAuto(), *(unsigned long*)&time_loaded);
 }

        //figure out how many days should be left
        int i_temp = C_DEMO_DAYS;
        
        i_temp -= GetDaysSinceDate(static_cast<time_t>(time_loaded.GetTime()));
        int i_total = C_DEMO_DAYS;
        
        if (i_temp > i_total) i_temp = 0; //they cheated somehow
        if (i_temp <= 0) i_temp = 0; //past expiration
         
        app_glo.SetTrialDaysLeft(i_temp); //trial period is OVER, cheaters!
        
        return i_temp;
}
