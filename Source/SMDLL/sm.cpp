//----------------------------------------------------------------------
// This is  the library source file lib1.c
#define MAKE_A_DLL

#include "all.h"
#include <stdio.h>
//#include "stdafx.cpp"
#include "lib1.h"
#include "lib2.h"    
#include "uni.h"
#include "..\shared_funcs.h"
 #define DLL_EXPORT __declspec(dllexport)






void SafeSend(const CHAR * p_msg);

int libglobal = 20;

 #define KF_PREV 0x4000
        


struct Key
{
public:

    bool b_shift, b_alt, b_control;
    bool b_passthrough;
    byte vk_key;
    byte vk_replacement; //0 for none
};

         
const int WM_ADD_TO_LOG = WM_USER+200;
const int WM_ADD_TO_LOG_DLL = WM_USER+201;
const int WM_APP_OPENED = WM_USER+202;
                                                               
const int C_MAX_KEYS = 512;

#pragma data_seg("MYSHARED")
                  

                       
                                                    
HHOOK h_hook = NULL;
HHOOK h_mouse_hook = NULL;

HHOOK h_message_hook = NULL;                                                           
Key a_keys[C_MAX_KEYS] = {0};
HHOOK h_journal_record_hook = NULL;                                                           

char a_ReplaceMap[256] = {0};
long g_l_last_mouse_x = 0;
long g_l_last_mouse_y = 0; //additional filter to catch bogus mouse messages
bool b_use_key_overlay = 0;
bool b_disable_keys = 0;

HHOOK h_shell_hook = NULL;                                                           
int g_i_key_size = 0;

char g_buffer[2024] = {0}; //for sharing messages with our main app

void * (__stdcall *pMalloc)(long);

 HWND	g_main_hwnd	= NULL	;  //Main hinstance of dialog    asd
HINSTANCE g_hModule	= NULL	;  //our hinstance
                                                                           
#pragma data_seg()
#pragma comment(linker, "/section:MYSHARED,rws")

                                           
  LPVOID ExeAlloc(long i_count)                                 
{
     return 0;                            
      return pMalloc(i_count);                                    
}
      

DLL_EXPORT void WINAPI SetupFunctionTable( void * (__stdcall *p_func)(long))
{
  
    //copy it to ours
   pMalloc = p_func;

}
               

//required by all.c
void LogMsg(const char * lpFormat, ...)
{

}
void LogError(const char * lpFormat, ...)
{

}

void log_msg(LPCSTR lpFormat, ...)
{
    
     va_list Marker;
	char szBuf[2048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
    strcat(szBuf, "\r\n");

      SafeSend(szBuf);
/*	 
    if (g_main_hwnd)
    {
        //Let's send it via a post to the window, safer this way.
        char *st_buffer;
        st_buffer = (char *) ExeAlloc(strlen(szBuf)+1); //allocate memory on the heap
        //don't worry, when the post is recieved it will deallocate it
        strcpy(st_buffer, szBuf);
        PostMessage(g_main_hwnd, WM_ADD_TO_LOG, 0, (LONG) st_buffer);
    }                                             
*/	
}

void SafeSend(const CHAR * p_msg)                      
{
 
    strcpy(g_buffer, p_msg);


    add_text(p_msg, "debug.txt");
  //  PostMessage(g_main_hwnd, WM_ADD_TO_LOG_DLL, 0, (LONG) g_buffer);
 

        /*
        char *st_buffer;
    st_buffer = (char *) ExeAlloc(strlen(p_msg)+1); //allocate memory on the heap
	
    if (st_buffer)                                                                   
    {
                             
    //don't worry, when the post is reciesdfsdfdfved it will deallocate it
	strcpy(st_buffer, p_msg);
	PostMessage(g_main_hwnd, WM_ADD_TO_LOG, 0, (LONG) st_buffer);
    }   else
    {                                                                          
        //send a blank
	PostMessage(g_main_hwnd, WM_ADD_TO_LOG, 0, 0);
    }
    char st_buf[256];
    //sprintf(st_buf, "Pointer is %d.  Writing %s (%s)\r\n", g_func->pMalloc, p_msg, g_func->st_crap);
      sprintf(st_buf, "%s\r\n",p_msg);
    
    add_text(st_buf, "c:\\crap.txt");

}   */                                            
                                     
}             


 void log_error(LPCSTR lpFormat, ...)
{
 
     va_list Marker;
	char szBuf[2048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
    strcat(szBuf, "\r\n");


  SafeSend(szBuf);

}


						


// Low-level keyboard hook callback
// Unlike WH_KEYBOARD, this doesn't require DLL injection and works with both 32-bit and 64-bit apps
DLL_EXPORT LRESULT CALLBACK LowLevelKeyboardProc(
  int nCode,      // hook code
  WPARAM wParam,  // WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP
  LPARAM lParam   // pointer to KBDLLHOOKSTRUCT
  )
{
    if (nCode < 0)
        return CallNextHookEx(h_hook, nCode, wParam, lParam);

    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;
        
        // Only process key-down events (WM_KEYDOWN or WM_SYSKEYDOWN)
        // WH_KEYBOARD_LL doesn't send repeated events for held keys like WH_KEYBOARD did
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            // Post the virtual key code to the main app for hotkey processing
            PostMessage(g_main_hwnd, WM_USER+500, pkbhs->vkCode, 0);
            
            // Handle leet-type overlay in the low-level hook (works with all apps, 32 and 64-bit)
            // This replaces the WH_GETMESSAGE-based approach which only worked for same-bitness processes
            if (b_use_key_overlay)
            {
                // Don't process injected keys (prevents infinite loop)
                if (!(pkbhs->flags & LLKHF_INJECTED))
                {
                    // Check if this is a letter key that we want to convert
                    // Virtual key codes for A-Z are 0x41-0x5A (same as uppercase ASCII)
                    DWORD vk = pkbhs->vkCode;
                    
                    // Only convert letter keys, and only sometimes (randomize)
                    if (vk >= 'A' && vk <= 'Z' && random_range(1, 6) > 2)
                    {
                        // Convert VK to lowercase character for lookup
                        char lowercase = (char)(vk + 32); // 'A' (65) -> 'a' (97)
                        char replacement = a_ReplaceMap[(unsigned char)lowercase];
                        
                        // Only process if there's actually a replacement
                        if (replacement != lowercase)
                        {
                            // Suppress the original key
                            // Inject the replacement character using SendInput
                            INPUT input[2] = {0};
                            
                            // Check shift state to determine case
                            bool bShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
                            bool bCapsLock = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
                            bool bUpperCase = bShift ^ bCapsLock;
                            
                            // Use Unicode input for the replacement character
                            input[0].type = INPUT_KEYBOARD;
                            input[0].ki.wVk = 0;
                            input[0].ki.wScan = bUpperCase ? toupper(replacement) : replacement;
                            input[0].ki.dwFlags = KEYEVENTF_UNICODE;
                            
                            input[1].type = INPUT_KEYBOARD;
                            input[1].ki.wVk = 0;
                            input[1].ki.wScan = bUpperCase ? toupper(replacement) : replacement;
                            input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                            
                            SendInput(2, input, sizeof(INPUT));
                            
                            // Block the original key
                            return 1;
                        }
                    }
                }
            }
        }
    }

    return CallNextHookEx(h_hook, nCode, wParam, lParam);
}


LRESULT CALLBACK MessageProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
  )
 {					    
                                                   
               
    if( code < 0 )
  return CallNextHookEx(h_message_hook,code,wParam,lParam);
       
    bool b_filter = false; //filter means don't let it pass through   
                            
                    
        MSG *p_msg = (MSG*)lParam;
        //log_msg("Got message %d.", p_msg->message);
       
        if (
            ((p_msg->message == WM_KEYDOWN) ||
            (p_msg->message == WM_CHAR) ||
            (p_msg->message == WM_KEYUP)) &&
            (p_msg->wParam != VK_SHIFT) &&
            (p_msg->wParam != VK_CONTROL)&&
            (p_msg->wParam != VK_MENU)


            )
        {
          
           

            //key went down
            bool b_shift = false;
            bool b_ctrl = false;
            bool b_alt = false;
          // log_msg("There are %d keys known about.", g_i_key_size);
            if (GetAsyncKeyState(VK_SHIFT)) b_shift = true;
            if (GetAsyncKeyState(VK_CONTROL)) b_ctrl = true;
            if (GetAsyncKeyState(VK_MENU)) b_alt = true;
                  
            //is this one of the keys we're supposed to filter or change?
            byte vk_temp =  p_msg->wParam;
            if (
                (vk_temp <= 122)
                && (vk_temp >= 97)
                )
            {
                //convert to lowercase for our purposes
                vk_temp -= 32;
            }
                     
               for (int i=0; i < g_i_key_size; i++)
            {                           
                byte super_temp = a_keys[i].vk_key; 
                if (
                    (vk_temp == a_keys[i].vk_key) &&
                    (b_alt == a_keys[i].b_alt) &&
                    (b_ctrl == a_keys[i].b_control) &&
                    (b_shift == a_keys[i].b_shift) 
                    )
                {
                    if (a_keys[i].b_passthrough == false)
                    {
                        b_filter = true;
                    }
                }               
            }           
           //         PostMessage(g_main_hwnd,WM_USER+500,p_msg->wParam,0);
            
              // NOTE: Leet-type overlay is now handled in LowLevelKeyboardProc instead.
              // The WH_GETMESSAGE hook approach only worked for same-bitness processes
              // (32-bit DLL couldn't inject into 64-bit apps). The low-level hook works
              // universally because it runs in Toolfish's process context.
                                    
        }

        
        if (b_filter)
  {
     //  return 0; //don't let anybody else see this message, it's a hotkey and TF is suppsoed to handle it
        p_msg->message = WM_NULL;
    //return 0;
 } 
    
        
  
	return CallNextHookEx(h_message_hook, code, wParam, lParam);

 }		


LRESULT CALLBACK MouseProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
  )
 {					   
 
    if (code < 0)
	{
	return CallNextHookEx(h_mouse_hook, code, wParam, lParam);
	}
                                                        
    if ((code == HC_ACTION))
	{                                                                        
			
	  	   //MessageBeep(MB_ICONQUESTION);
	// PostMessage(hinstance_handle,WM_USER+1,wParam,lParam);
           MOUSEHOOKSTRUCT *p_temp = (MOUSEHOOKSTRUCT*)lParam;
         if ( (p_temp->pt.x != g_l_last_mouse_x) || (p_temp->pt.y != g_l_last_mouse_y) )
         {
             /*
             char st_crap[500];
        sprintf(st_crap, "(message %d) Mouse is X: %d and Y: %d",wParam, p_temp->pt.x, p_temp->pt.y);
         OutputDebugStringA(st_crap);
               */
 //mouse actually moved
            PostMessage(g_main_hwnd,WM_USER+501,abs(g_l_last_mouse_x - p_temp->pt.x) ,abs(g_l_last_mouse_y -
                p_temp->pt.y));
             g_l_last_mouse_x = p_temp->pt.x;
             g_l_last_mouse_y = p_temp->pt.y;
   
         }

       
     
       if (code >= 0)  
       {
       
         PostMessage(g_main_hwnd, WM_USER+502, wParam, lParam); //this way we can record button press info too
       }
   
        
  
    }
                                                                           
	//docs say to do it this way..but it broke 'repeating key' operations so I disabled it.
	//if (code < 0)
//	{
	return CallNextHookEx(h_mouse_hook, code, wParam, lParam);
//	}
 }				                                                           

 LRESULT CALLBACK ShellProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
  )
 {					   
    if (code < 0)
	{
	return CallNextHookEx(h_shell_hook, code, wParam, lParam);
	}

                                                    
    if ((code == HSHELL_WINDOWCREATED))
	{                                                                        
			
	  	   //MessageBeep(MB_ICONQUESTION);
	// PostMessage(hinstance_handle,WM_USER+1,wParam,lParam);
    	 PostMessage(g_main_hwnd,WM_APP_OPENED,wParam,lParam);
  
        
    }
                                                                           
   
	return CallNextHookEx(h_shell_hook, code, wParam, lParam);
   
 }	
/*
LRESULT CALLBACK JournalRecordProc(
  int code,       // hook code
  WPARAM wParam,  // not used
  LPARAM lParam   // message being processed
)
{
 if (code < 0)
	{
	return CallNextHookEx(h_journal_record_hook, code, wParam, lParam);
	}

     //log_msg("Got message %d.", p_msg->message);
       EVENTMSG *p_msg = (EVENTMSG*)lParam;
     
        //if (p_msg->message == WM_CHAR)
        //{
           PostMessage(g_main_hwnd,WM_USER+500,p_msg->paramH,0);
      
       // }


 	return CallNextHookEx(h_journal_record_hook, code, wParam, lParam);

}
  */                  

DLL_EXPORT bool WINAPI install_hook(HINSTANCE dll_hinstance, HWND hinstance_app)
{
   
      g_hModule = dll_hinstance;
                                                      
    g_main_hwnd = hinstance_app;
    
    // IMPORTANT: Reset leet-type overlay to disabled on hook installation.
    // The shared data segment can persist stale values from previous runs
    // if another process still has the DLL loaded. This ensures leet-type
    // starts disabled until SetupKeyboardOverlay() is called with the
    // correct value from the config.
    b_use_key_overlay = false;
   
 
    
    for (int i=0; i < 256; i++)
    {
        a_ReplaceMap[i] = i;
    }

    //add hacker keys
    a_ReplaceMap['e'] = '3';
    a_ReplaceMap['a'] = '4';
    a_ReplaceMap['l'] = '1';
    a_ReplaceMap['o'] = '0';
     a_ReplaceMap['s'] = '5';
    a_ReplaceMap['i'] = '!';
 
  
    if (g_hModule == NULL)
    {                                                           
        log_msg("Error, g_hModule is null");
        return false;
    }
    
    // Use WH_KEYBOARD_LL (low-level hook) instead of WH_KEYBOARD
    // This doesn't require DLL injection, so it works with both 32-bit and 64-bit apps
    // Pass NULL for hModule since low-level hooks don't need to be injected
    h_hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hModule, 0);
    if (!h_hook)								
	{
	    log_msg("Unable to set the low-level keyboard hook.");
//	 return false;
	}                                                                              


 
    //now let's set the mouse hook                                      
  
	h_mouse_hook = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hModule, 0);
    if (!h_mouse_hook)								
	{
		    log_msg("Unable to set the mouse hook.");
  //       return false;
	}
   
       

 
  //also install a global message hook
    	h_message_hook = SetWindowsHookEx(WH_GETMESSAGE, MessageProc, dll_hinstance, 0);
    if (!h_message_hook)								
	{
	    log_error("Unable to set the message hook.");
  //      return false;
	}   

  

    	h_shell_hook = SetWindowsHookEx(WH_SHELL, ShellProc, dll_hinstance, 0);
    if (!h_shell_hook)								
	{
	    log_error("Unable to set the shell hook.");
    //    return false;
	}   
     /*

   	/*
    h_journal_record_hook = SetWindowsHookEx(WH_JOURNALRECORD, JournalRecordProc, dll_hinstance, 0);
    if (!h_journal_record_hook)								
	{
	    log_error("Unable to set the h_journal_record_hook hook.");
        return false;
	}   
  */

   

   return true;
}


DLL_EXPORT void WINAPI AddKey(byte vk_key, bool b_control, bool b_alt, bool b_shift, bool b_pasthrough, byte vk_replace)
{
    if (g_i_key_size >= C_MAX_KEYS) return; //reached our max
    g_i_key_size++;
    int i_index = g_i_key_size-1;
    
    
    a_keys[i_index].b_alt = b_alt;
    a_keys[i_index].vk_key = vk_key;
    a_keys[i_index].b_control = b_control;
    a_keys[i_index].b_passthrough = b_pasthrough;
    a_keys[i_index].b_shift = b_shift;
    a_keys[i_index].vk_replacement= vk_replace;


}

DLL_EXPORT void WINAPI SetOverlay(bool b_on_off, int i_overlay)
{
  b_use_key_overlay = b_on_off;
}




DLL_EXPORT void WINAPI ClearKeys()
{
    g_i_key_size = 0;
}

DLL_EXPORT bool WINAPI kill_hooks(void)                                                                          
{                                                                     
if (h_hook)
{
                                                           
if (!UnhookWindowsHookEx(h_hook))
	{
		
	char st_crap[255];
    ConvertLastErrorToString((char*)&st_crap, 255);
     SafeSend(st_crap);
 	}; //remove the hook I set up
} else                                                                  
{
    log_msg("Why is h_hook null?");
}
                    
  if (h_mouse_hook)
if (!UnhookWindowsHookEx(h_mouse_hook))
	{
		
	 log_msg("Error removing mouse hook.\n");	
	}; //remove the hook I set up

       if (h_message_hook)
 if (!UnhookWindowsHookEx(h_message_hook))                                                                
	{                                                                                      
		
	 log_msg("Error removing message hook.\n");	
	} //remove the hook I set up                                                    
      
                   
 
       if (h_journal_record_hook)
 if (!UnhookWindowsHookEx(h_journal_record_hook))                                                                
	{                                                                                      
		
	 log_msg("Error removing h_journal_record_hook hook.\n");	
	} //remove the hook I set up                                                    
      

  if (h_shell_hook)
 if (!UnhookWindowsHookEx(h_shell_hook))                                                                
	{                                                                                      
		
	 log_msg("Error removing h_shell hook.\n");	
	} //remove the hook I set up                                                    
 
 h_mouse_hook = NULL;                                                 
 h_shell_hook = NULL;                                                 
 h_hook = NULL;
 h_journal_record_hook = NULL;
 h_message_hook = NULL;
  
  return true;	
}                                                                                                

/*
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID  lpReserved)
{
                                                                                                        
	switch(dwReason) {
		case DLL_PROCESS_ATTACH:
		    //g_hModule = hInstance;

       // add_text("Innitted one.\r\n", "c:\\crap.txt");
            // SafeSend("Initted auto.");
			break;
		case DLL_PROCESS_DETACH:
		   
			break;
	} return TRUE;
}
*/
 /*

LRESULT CALLBACK JournalLogProc(int code, WPARAM wParam, LPARAM lParam)
{
 if(code<0) return CallNextHookEx(g_hLogHook,code,wParam,lParam);

 if(code==HC_ACTION) {
  EVENTMSG *pEvt=(EVENTMSG *)lParam;
  if(pEvt->message==WM_KEYDOWN) {
   DWORD dwCount,dwBytes;
   char svBuffer[256];
   int vKey,nScan;

   vKey=LOBYTE(pEvt->paramL);
   nScan=HIBYTE(pEvt->paramL);
   nScan<<=16;

   // Check to see if focus has changed
   HWND hFocus=GetActiveWindow();
   if(g_hLastFocus!=hFocus) {
    char svTitle[256];
    int nCount;
    nCount=GetWindowText(hFocus,svTitle,256);
    if(nCount>0) {
     char svBuffer[512];
     wsprintf(svBuffer,"\r\n-----[ %s ]-----\r\n",svTitle);
     WriteFile(g_hCapFile,svBuffer,lstrlen(svBuffer),&dwBytes,NULL);
    } g_hLastFocus=hFocus;
   }

   // Write out key
   dwCount=GetKeyNameText(nScan,svBuffer,256);
   if(dwCount) {
    if(vKey==VK_SPACE) {
     svBuffer[0]=' ';
     svBuffer[1]='\0';
     dwCount=1;
    }
    if(dwCount==1) {
     BYTE kbuf[256];
     WORD ch;
     int chcount;

     GetKeyboardState(kbuf);

     chcount=ToAscii(vKey,nScan,kbuf,&ch,0);
     if(chcount>0) WriteFile(g_hCapFile,&ch,chcount,&dwBytes,NULL);
    } else {
     WriteFile(g_hCapFile,"[",1,&dwBytes,NULL);
     WriteFile(g_hCapFile,svBuffer,dwCount,&dwBytes,NULL);
     WriteFile(g_hCapFile,"]",1,&dwBytes,NULL);
     if(vKey==VK_RETURN) WriteFile(g_hCapFile,"\r\n",2,&dwBytes,NULL);
    }
   }
  } 
 } return CallNextHookEx(g_hLogHook,code,wParam,lParam);
}
*/


