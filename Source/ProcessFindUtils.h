#pragma once

#include "CGlobals.h"
class CEvent;

bool ProcessProcessTrigger( CEvent *p_event, int i_index);
TCHAR* GetExeFromHwnd(HWND hWnd, TCHAR* buf);
DWORD ScanProcessesForApp(TCHAR * p_t_text);
void SendCommandToApp( CEvent *p_event, int i_index);
void SendMessageToWindow( HWND hwnd, CEvent *p_event, int i_index, DWORD pid);
void BuildListOfActiveProcesses(CString & cst_text);
