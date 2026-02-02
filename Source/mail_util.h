#pragma once
#include "CGlobals.h"
#include "CEvent.h"
#include "CEventList.h" 
#include "..\Shared\pop3.h"

#include <vector>

using namespace std;

typedef vector<CMailMessage*> MailVec;

bool ProcessEmail(CEvent *p_event, int i_index);
bool SendEmail(TCHAR *p_content, CEvent *p_event, int i_index);


