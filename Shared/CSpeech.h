//Seth's Microsoft Speech wrapper
#pragma once


#include "all.h"
#include "sapi.h"


class CSpeech
{
  
private:
	
	ISpVoice * pVoice;
	bool b_muted;

public:
	CSpeech();
#ifdef _UNICODE
    void CSpeech::say(const TCHAR p_tchar[]); //unicode
#endif
    void CSpeech::set_mute(bool b_new);
  ~CSpeech();
void CSpeech::say(const LPCSTR lpFormat, ...);
void CSpeech::say(const WCHAR p_tchar[], DWORD dw_options);

};


