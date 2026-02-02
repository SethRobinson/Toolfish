#include "ProcessFindUtils.h"
#include "CEvent.h"
#include "CTrigger.h"
#include <Tlhelp32.h>
#include "CAction.h"
#include "spy_util.h"

#define TA_FAILED 0
#define TA_SUCCESS_CLEAN 1
#define TA_SUCCESS_KILL 2
#define TA_SUCCESS_16 3


class EnumWindowInfo
{
public:

    EnumWindowInfo()
    {
        //set to default values
        memset(this, 0, sizeof(EnumWindowInfo));
    }

    HWND hWnd_window; 
    TCHAR t_search_text[512]; //what to look for
    DWORD pid;
};



BOOL CALLBACK EnumWindowFindProc( HWND hwnd, LPARAM p_user_defined)
{
    EnumWindowInfo *p_info = (EnumWindowInfo*)p_user_defined;

    TCHAR wst_win_text[512];
    GetWindowText(hwnd, wst_win_text, 512);
    if (wst_win_text[0] != 0)
    {
     
        if (stristr(wst_win_text, p_info->t_search_text) != 0)
        {
          //found match
            p_info->hWnd_window = hwnd;
            return false; //all done
        }
    }

    return true; 
}

bool ProcessProcessTrigger( CEvent *p_event, int i_index)
{
    bool b_found = false;
    
    DWORD pId = 0;
    
    
    if (p_event->GetTrigger(i_index)->GetProcessFocus())
    {
        //only check the window that has focus
        TCHAR t_temp[512];
        
        
        
        HWND hwnd_temp = GetForegroundWindow();
       HWND new_hWnd = GetWindow(hwnd_temp, GW_OWNER); 
        
            if (new_hWnd)
            {
                hwnd_temp = new_hWnd;
            }

        
        if (p_event->GetTrigger(i_index)->GetProcessSearchField() == C_TRIGGER_PROCESS_FILENAME)
        {
            //check filename
            GetExeFromHwnd(hwnd_temp, t_temp);
            //LogMsg(_T("Focus window is %s"), t_temp);
            
#ifdef _UNICODE
            if (_tcsicmp( t_temp, p_event->GetTrigger(i_index)->GetBufferPointer()) == 0)
#else
            if (_tcsicmp( t_temp, uni(p_event->GetTrigger(i_index)->GetBufferPointer()).st_data) == 0)

#endif
            {
                //match!
               b_found = true;
            }   else
            {
              //all done, no match made
            }
            
            
        }
        
        
        if (p_event->GetTrigger(i_index)->GetProcessSearchField() == C_TRIGGER_PROCESS_WINDOW_TEXT)
        {
            
            
            GetWindowText(hwnd_temp, t_temp, 512);
            if (t_temp[0] != 0)
            {
                
#ifdef _UNICODE
                if (stristr( t_temp, p_event->GetTrigger(i_index)->GetBufferPointer()) != 0)
#else
                    if (stristr( t_temp, uni(p_event->GetTrigger(i_index)->GetBufferPointer()).to_st()) != 0)
                        
#endif
                    {
                        //found match
                        b_found = true;
                        
                        
                    } else
                    {
                        //no match made
                        
                    }
            } else
            {
                //window text is blank, assume it's not it
                
            }
        }
    } else
    {
        
        
        
        
        
        if (p_event->GetTrigger(i_index)->GetProcessSearchField() == C_TRIGGER_PROCESS_FILENAME)
        {
#ifdef _UNICODE
            pId = ScanProcessesForApp(p_event->GetTrigger(i_index)->GetBufferPointer());
#else
            pId = ScanProcessesForApp(uni(p_event->GetTrigger(i_index)->GetBufferPointer()).st_data);
            
#endif
            if (pId)
            {
                
                b_found = true;
            }
            
            
        } else
        {
            //search by window title.  a bit slower actually
            EnumWindowInfo info;
#ifdef _UNICODE
            _tcscpy(info.t_search_text, p_event->GetTrigger(i_index)->GetBufferPointer());
#else
            _tcscpy(info.t_search_text, uni(p_event->GetTrigger(i_index)->GetBufferPointer()).st_data);
#endif
            if (!EnumWindows(EnumWindowFindProc, (LPARAM)&info))
            {
                // LogError(_T("Error enumming windows."));
                //      return false;
            }
            
            
            if (info.hWnd_window != 0)
            {
                //found match
                b_found = true;
            }
            
            
        }
        
    }
  
  if (p_event->GetTrigger(i_index)->GetProcessFoundField() == C_TRIGGER_PROCESS_NOT_EXIST)  
  {
      //only return true if it DOESN'T exist
      b_found = !b_found; 
  }
  return b_found; //couldn't find it
}



DWORD ScanProcessesForApp(TCHAR * p_t_text)
{

    PROCESSENTRY32 PData = {0};
    
 HANDLE hSnapshot=NULL;
 HANDLE hKernel = NULL;
 DWORD pid_final = 0;



 // Take the process snapshot
 hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
 if (hSnapshot == (HANDLE)-1)
 {
   log_error("CreateToolhelp32Snapshot failed.");
     return NULL;
 }

  PData.dwSize = sizeof(PROCESSENTRY32);
 
 
 //Look for our process, and get it's executable filename
    if (Process32First(hSnapshot, &PData))
 { 
        do
        { 
            if (_tcsicmp(PData.szExeFile, p_t_text) == 0)
            {
              
           // LogMsg(_T("%s"),PData.szExeFile );
             CloseHandle (hSnapshot); 
   
            return PData.th32ProcessID; 
            }
                    
            
            //    _tcscpy(buf, PData.szExeFile);
        } 
        while (Process32Next(hSnapshot, &PData) && pid_final == 0 );
       
    } 
    else 
    {
     log_error("ScanProcessesForApp could not walk the task list.");
     
    }
    
 // Do not forget to clean up the snapshot object. 
    CloseHandle (hSnapshot); 
    

  return 0; //failed to find jack and or crap

}



TCHAR* GetExeFromHwnd(HWND hWnd, TCHAR* buf)
{
 PROCESSENTRY32 PData;
 HANDLE hSnapshot=NULL;
 DWORD PId;
 BOOL bRet;
 HANDLE hKernel = NULL;

 // Toolhelp functions in KERNEL32.DLL
 
 // Get pointers to toolhelp32 functions in kernel module
 
 GetWindowThreadProcessId(hWnd, &PId);
 *buf='\0';

 PData.dwSize = sizeof(PData);

 // Take the process snapshot
 hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
 if (hSnapshot == (HANDLE)-1)
  return NULL;

 // Look for our process, and get it's executable filename
    if (Process32First(hSnapshot, &PData))
 { 
        do
        { 
            if (PData.th32ProcessID == PId)
                _tcscpy(buf, PData.szExeFile);
        } 
        while (Process32Next(hSnapshot, &PData) && !(*buf) );
        
        if (*buf)
            bRet = TRUE; 
        else
            bRet = FALSE;
    } 
    else 
        bRet = FALSE;    // could not walk the list of processes 
    
 // Do not forget to clean up the snapshot object. 
    CloseHandle (hSnapshot); 
    
 if (bRet)
  return buf; 
 else
  return NULL;

}



BOOL CALLBACK EnumWindowGetHwndFromPid( HWND hwnd, LPARAM p_user_defined)
{
    EnumWindowInfo *p_info = (EnumWindowInfo*)p_user_defined;
    DWORD pid = 0;
    
    GetWindowThreadProcessId(hwnd, &pid);
    
    if (p_info->pid == pid) 
    {
        
        //found match
        p_info->hWnd_window = hwnd;
        return false; //all done
    }
    
    
    return true; 
}



void SendCommandToApp( CEvent *p_event, int i_index)
{
 DWORD pId = 0;
     HWND hwnd_final = 0;
       
        if (p_event->GetAction(i_index)->GetProcessSearchField() == C_ACTION_PROCESS_FILENAME)
        {


#ifdef _UNICODE
            pId = ScanProcessesForApp(p_event->GetAction(i_index)->GetBufferPointer());
#else
            pId = ScanProcessesForApp(uni(p_event->GetAction(i_index)->GetBufferPointer()).st_data);
            
#endif
            if (pId)
            {
              EnumWindowInfo info;
              info.pid = pId;
 
              if (!EnumWindows(EnumWindowGetHwndFromPid, (LPARAM)&info))
            {
                 
                  // LogError(_T("Error enumming windows."));
                  //      return false;
                  
              }
              hwnd_final =  info.hWnd_window;
              
            }
            
        } else
        {
            //search by window title.  a bit slower actually
            EnumWindowInfo info;
#ifdef _UNICODE
            _tcscpy(info.t_search_text, p_event->GetAction(i_index)->GetBufferPointer());
#else
            _tcscpy(info.t_search_text, uni(p_event->GetAction(i_index)->GetBufferPointer()).st_data);
#endif
            if (!EnumWindows(EnumWindowFindProc, (LPARAM)&info))
            {
                // LogError(_T("Error enumming windows."));
                //      return false;
            }
            hwnd_final =  info.hWnd_window;
            if (hwnd_final)
            {
                GetWindowThreadProcessId(hwnd_final, &pId);
   
            }
        }
        
        
        
        if (hwnd_final)
        {
            //let's actually send the message
#ifdef _DEBUG
            LogMsg(_T("Got %x - pid %d"), hwnd_final, pId);
#endif
         SendMessageToWindow(hwnd_final, p_event, i_index, pId);
            
            
        }  else
        {
            //error
            if (p_event->GetAction(i_index)->GetProcessSearchField() == C_ACTION_PROCESS_FILENAME)
            {
                if (!p_event->GetQuietMode())
                {
                
                log_msg("Send Command To Application: Error finding a process that had %s as a filename, aborting.",
                    uni(p_event->GetAction(i_index)->GetBufferPointer()).st_data);
                }
                
            }
            
            if (p_event->GetAction(i_index)->GetProcessSearchField() == C_ACTION_PROCESS_WINDOW_TEXT)
            {
                if (!p_event->GetQuietMode())
                {
                log_error("Send Command To Application error: Error finding a process that had %s in the window caption, aborting.",
                    uni(p_event->GetAction(i_index)->GetBufferPointer()).st_data);
                }
                
            }
            
        }
        
}



 typedef struct
   {
      DWORD   dwID ;
      DWORD   dwThread ;
   } TERMINFO ;

   // Declare Callback Enum Functions.
   BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam ) ;

  
  
   BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
   {
      DWORD dwID ;

      GetWindowThreadProcessId(hwnd, &dwID) ;

      if(dwID == (DWORD)lParam)
      {
         PostMessage(hwnd, WM_CLOSE, 0, 0) ;
      }

      return TRUE ;
   }


   
   /*----------------------------------------------------------------
   DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )

   Purpose:
      Shut down a 32-Bit Process (or 16-bit process under Windows 95)

   Parameters:
      dwPID
         Process ID of the process to shut down.

      dwTimeout
         Wait time in milliseconds before shutting down the process.

   Return Value:
      TA_FAILED - If the shutdown failed.
      TA_SUCCESS_CLEAN - If the process was shutdown using WM_CLOSE.
      TA_SUCCESS_KILL - if the process was shut down with
         TerminateProcess().
      NOTE:  See header for these defines.
   ----------------------------------------------------------------*/ 
   DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout, bool b_task_kill )
   {
      HANDLE   hProc ;
      DWORD   dwRet ;

      // If we can't open the process with PROCESS_TERMINATE rights,
      // then we give up immediately.
      hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,
         dwPID);

      if(hProc == NULL)
      {
         return TA_FAILED ;
      }

      // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
      // matches your process's.
      EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID) ;

      if (b_task_kill)
      {
      
      // Wait on the handle. If it signals, great. If it times out,
      // then you kill it.
      if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
         dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
      else
         dwRet = TA_SUCCESS_CLEAN ;
      } else
      {
         dwRet = TA_SUCCESS_CLEAN ;

      }

      CloseHandle(hProc) ;

      return dwRet ;
   }




void SendStandardCommand( HWND hwnd, CEvent *p_event, int i_index, DWORD pid)
{
  
    TCHAR t_mes[512];
    TCHAR t_action[128];
    TCHAR t_type[64];
  
    switch (p_event->GetAction(i_index)->GetProcessWMCommand())
    {
  
case C_ACTION_WM_QUIT_HARD:
    {
        _stprintf(t_action, _T("Closing application, will do an end-task in 15 seconds if required"));
        // PostMessage(hwnd, WM_QUIT, 0,0);
        TerminateApp(pid, 15000, true);
        break;
    }
case C_ACTION_WM_QUIT:
    {
        _stprintf(t_action, _T("Closing application"));
     // PostMessage(hwnd, WM_QUIT, 0,0);
        TerminateApp(pid, 0, false);
        break;
    }

case C_ACTION_WM_FOCUS:
    {
         _stprintf(t_action, _T("Setting focus to application"));
      SetForegroundWindow(hwnd);
      break;
    }

case C_ACTION_WM_MINIMIZE:
    {
         _stprintf(t_action, _T("Minimizing application"));
      CloseWindow(hwnd);
      break;
    }

case C_ACTION_WM_MAXIMIZE:
    {
        _stprintf(t_action, _T("Maximizing application"));
        ShowWindow(hwnd, SW_MAXIMIZE);
        break;
    }

case C_ACTION_WM_MOUSE_LEFT_CLICK:
	{
		_stprintf(t_action, _T("Faking left mouse click in application"));

		// Clicking using Win32 APIs, with the control handle and the coordinates relative to it as params
		LPARAM lParam = MAKELPARAM(500, 300);
		SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
		SendMessage(hwnd, WM_LBUTTONUP, 0, lParam);

		break;
	}


    
case C_ACTION_WM_OK:
    {
           _stprintf(t_action, _T("Faking OK button in application"));
      
        PostMessage(hwnd, WM_COMMAND, (BN_CLICKED << 16)|IDOK, 0);
        break;
        
    }
case C_ACTION_WM_CANCEL:
    {
           _stprintf(t_action, _T("Faking CANCEL button in application"));
        
        PostMessage(hwnd, WM_COMMAND, (BN_CLICKED << 16)|IDCANCEL, 0);
        break;
        
    }
   
default:
    assert(!"Unknown action.");
    LogError(_T("Don't know how to handle this send message command.  You may need to upgrade?"));
    _stprintf(t_action, _T("Not doing anything to application"));
    
    }


   if (p_event->GetAction(i_index)->GetProcessSearchField() == C_ACTION_PROCESS_FILENAME)
   {
       _stprintf(t_type, _T("as the filename."));
   }

   if (p_event->GetAction(i_index)->GetProcessSearchField() == C_ACTION_PROCESS_WINDOW_TEXT)
   {
       _stprintf(t_type, _T("in the window status bar text."));
   }

   //compose final message

  if (!p_event->GetQuietMode())
  {
  
#ifdef _UNICODE
   _stprintf(t_mes, _T("%s with \"%s\" %s"), t_action, 
     p_event->GetAction(i_index)->GetBufferPointer(), t_type);
#else
   
   _stprintf(t_mes, _T("%s with \"%s\" %s"), t_action, 
     uni(p_event->GetAction(i_index)->GetBufferPointer()).st_data, t_type);
#endif
   LogMsg(t_mes);
  }

}


void InjectCharRelease(char vk, WORD w_special)
{
  INPUT input;
  memset(&input, 0, sizeof(INPUT));
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = vk;
  input.ki.wScan = w_special;
  input.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &input, sizeof(INPUT));
}
 


void InjectCharPress(char vk, WORD w_special)
{
  INPUT input;
  memset(&input, 0, sizeof(INPUT));
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = vk;
  input.ki.wScan = w_special;
  //input.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &input, sizeof(INPUT));
}

     
void InjectChar(char ch_char, bool b_shift, bool b_ctrl, bool b_alt)
{
  //save original states of special keys
  bool b_orig_shift = false;
  bool b_orig_ctrl = false;
  bool b_orig_alt = false;
  
 if (GetAsyncKeyState(VK_SHIFT))
 {
     InjectCharRelease(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0));
     b_orig_shift = true;
 }
 if (GetAsyncKeyState(VK_MENU))
 {
     InjectCharRelease(VK_MENU, MapVirtualKey(VK_MENU, 0));
      b_orig_alt = true;
 }
 if (GetAsyncKeyState(VK_CONTROL))
 {
     InjectCharRelease(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0));
      b_orig_ctrl = true;
 }
 

 //now turn on the ones we're supposed to
 if (b_shift)
 {
     InjectCharPress(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0));
 }

if (b_alt)
 {
     InjectCharPress(VK_MENU, MapVirtualKey(VK_MENU, 0));
 }
if (b_ctrl)
 {
     InjectCharPress(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0));
 }
 
 //now send the real keystroke
  InjectCharPress(char(VkKeyScan(ch_char)), 0);
  InjectCharRelease(char(VkKeyScan(ch_char)), 0);

  //now release any specual keys we had down

if (b_shift)
 {
     InjectCharRelease(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0));
 }

if (b_alt)
 {
     InjectCharRelease(VK_MENU, MapVirtualKey(VK_MENU, 0));
 }
if (b_ctrl)
 {
     InjectCharRelease(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0));
 }


//now put back the defaults if required

 if (b_orig_shift)
 {
     InjectCharPress(VK_SHIFT, MapVirtualKey(VK_SHIFT, 0));
 }

if (b_orig_alt)
 {
     InjectCharPress(VK_MENU, MapVirtualKey(VK_MENU, 0));
 }
if (b_orig_ctrl)
 {
     InjectCharPress(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0));
 }


}


void FakeKeystroke( HWND hwnd, CEvent *p_event, int i_index, DWORD pid)
{
    //save current focus info
    HWND h_old_focus = GetForegroundWindow();
    
    //first switch focus to thing we're going to send to
    SetForegroundWindow(hwnd);
    
    //send the keystroke with a low level keyboard hit event
    InjectChar(p_event->GetAction(i_index)->GetVK(),p_event->GetAction(i_index)->GetShift(),
        p_event->GetAction(i_index)->GetCtrl(),p_event->GetAction(i_index)->GetAlt());
    
    //make sure the keys have time to send
    Sleep(10);

    
    //return focus to where we were originally
    SetForegroundWindow(h_old_focus);
    

}

//let's actually send a message to an outside (presumably) window

void SendMessageToWindow( HWND hwnd, CEvent *p_event, int i_index, DWORD pid)
{
  
    if (p_event->GetAction(i_index)->GetProcessMessageType() == C_ACTION_MESSAGE_NORMAL)
    {
    
    SendStandardCommand(hwnd, p_event, i_index, pid);
    } else
    
    if (p_event->GetAction(i_index)->GetProcessMessageType() == C_ACTION_MESSAGE_KEY)
    {
    FakeKeystroke(hwnd, p_event, i_index, pid);
    } else
        if (p_event->GetAction(i_index)->GetProcessMessageType() == C_ACTION_MESSAGE_CUSTOM)
        {
            //send a custom message
       
             
                SendMessage(hwnd, p_event->GetAction(i_index)->GetCustomMsg(),
               p_event->GetAction(i_index)->GetCustomWParam(), p_event->GetAction(i_index)->GetCustomLParam());
        } else
     {
        log_error("Unsupported message type.");
    }
   
}

class EnumList
{
public:
    
    CString *p_cst_text;


};

 BOOL CALLBACK AppEnumList( HWND hwnd, LPARAM p_user_defined)
{
    EnumList *p_info = (EnumList*)p_user_defined;
    DWORD pid = 0;
    
   
    CString cst_temp;
    
    TCHAR t_window[256], t_exe[256];
    
    
    GetWindowText(hwnd, &t_window[0], 256);
    
    if ( (t_window[0] != 0) && _tcscmp(t_window, _T("Default IME")) != 0)
    {
     GetExeFromHwnd(hwnd, &t_exe[0]);
    cst_temp.Format(_T("%s - (Exe name is %s)\r\n"), t_window, t_exe);
    *p_info->p_cst_text += cst_temp;
    }
    return true; 
}
void BuildListOfActiveProcesses(CString & cst_text)
{
   EnumList elist;
   elist.p_cst_text = &cst_text;

    EnumWindows((WNDENUMPROC)AppEnumList, (LPARAM) &elist) ;


}
