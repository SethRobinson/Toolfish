#pragma once
#include "CGLobals.h"

void SetMasterVolume(unsigned long value);
unsigned long GetMasterVolume(unsigned long &min,  unsigned long &max);
void ProcessMuteKey();
void ProcessMuteMouse();
bool ProcessVolumeKnobKey(WPARAM wParam);

