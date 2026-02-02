#pragma once
#include "stdafx.h"
#include "CGlobals.h"
class CEvent; //forward for speed

void InitiateOSShutdown(UINT uTheRequestType);
bool ProcessReboot(CEvent *p_event, int i_index);
