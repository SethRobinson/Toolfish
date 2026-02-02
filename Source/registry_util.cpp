#include "stdafx.h"
#include "registry_util.h"
#include "CGlobals.h"
#include "..\Shared\registry.h"

bool global_registry( bool return_only, bool add )
{
 
 //check reg key for autoloading

	 CRegistry reg;
	 CString sRegPath("Software\\Microsoft\\Windows\\CurrentVersion\\Run");	

 		if (!reg.VerifyKey(HKEY_CURRENT_USER, sRegPath))
		{
			 log_error("Error - Run reg key doesn't exist!");
		  	 return false;
			
		}	
	
		if (!reg.VerifyValue(app_glo.GetAppName()))
		{

      	    //key does not exist
			if (!add)
			{
			 //we were here to delete, and we don't have to.  Get out!
				
				return false;
			}
			
			if (return_only)
			{

		//	 log_msg("Key doesn't exist.  But we aren't here to change that.");
			 return false;
			}
			
			reg.CreateKey(HKEY_CURRENT_USER, sRegPath);

		    TCHAR st_full_path[256];
		
            GetModuleFileName(NULL, st_full_path, 255);
    
            CString cst_regdata = st_full_path;
			cst_regdata.Replace(_T("\""),_T(""));
			cst_regdata += _T(" -minimize");
			
			reg.WriteString(app_glo.GetAppName(), cst_regdata); 
		}  else
		{
		 //key exists.
			
//			 log_msg("Key already exists.");
			if (return_only)
			{
			 return true;
			}

			if (!add)
			{
			  //let's delete the key.

			  reg.DeleteValue(app_glo.GetAppName());
			 // log_msg("Key deleted.\n");

			}
		}
	
    return true;
}

