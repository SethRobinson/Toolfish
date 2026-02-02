#pragma once
#include "CGlobals.h"
class CEvent; //for speed

void ProcessOpenedWindow(HWND h_win);
void ProcessGotKey(WPARAM wParam,  // virtual-key code
                   LPARAM lParam  ); 
void ReplaceStrings(TCHAR *p_dest, const TCHAR *p_src, int i_size_of_dest, int i_size_of_src, CEvent *p_event);
void InjectString(TCHAR *str, int len);
void SetupKeyboardOverlay();


