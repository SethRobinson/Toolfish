#include "CVoice.h"
#include "ProcessEvents.h"
#include "..\Shared\CSpeech.h"

void CVoice::SetDefaults()
{
    SetSpeed(0);
    SetPitch(0);
    SetVolume(100);
}


void CVoice::Say(const TCHAR * p_words)
{
    CString st_temp;
    st_temp.Format(_T("<pitch middle = '%d'><pitch absmiddle = '%d'><rate speed = '%d'>%s"),GetPitch(),GetPitch(), GetSpeed(),p_words);
#ifndef _UNLOCK_UTILITY_
    GlobalSay(st_temp);
#endif
    
}

//cuts off older text
void CVoice::SayTest(const TCHAR * p_words)
{
  #ifndef _UNLOCK_UTILITY_
    CString st_temp;
    st_temp.Format(_T("<pitch middle = '%d'><pitch absmiddle = '%d'><rate speed = '%d'>%s"),GetPitch(),GetPitch(), GetSpeed(),p_words);
    if (app_glo.GetSpeech())
    {
	
    app_glo.GetSpeech()->say(st_temp, SPF_IS_XML | SPF_ASYNC | SPF_PURGEBEFORESPEAK);
    } else
    {
        LogError(_T("Microsoft SAPI not initted, can't use text to speech. (check your options dialog)"));
    }
#endif
}
