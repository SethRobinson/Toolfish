#pragma once

#include "CGlobals.h"
class CEvent;

void ProcessTimerEvents(); 
void ProcessHotkeyEvents(char vk);
void RunEvent(CEvent *p_event);
void GlobalSay(const TCHAR *p_message);
void BuildHotKeyInfo(); 




