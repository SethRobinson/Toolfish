#include "CSpeech.h"
#include "Uni.h"

CSpeech::CSpeech()
{
#ifdef _DEBUG
LogMsg(_T("Initted speech system."));
#endif
    //init everything
 CoInitialize(NULL);
  pVoice = NULL;
  b_muted = false;

   HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
  		
   if( SUCCEEDED( hr ) )
   {
   }  else
   {
       LogError(_T("Error: You need to install the Microsoft Speech API!  To install it, go here:  http://www.rtsoft.com/speech"));
       LogError(_T("For now, the text to speech features will be disabled."));
       pVoice = NULL;
       return;
   }
   
   
   
}

void CSpeech::set_mute(bool b_new)
{
  b_muted = b_new;

}

	
void CSpeech::say(const LPCSTR lpFormat, ...)
{
	if ((!pVoice) || (b_muted)) return;

	
	va_list Marker;
	char szBuf[4048];
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	
	uni uni_text(szBuf);

 pVoice->Speak(uni_text.us_data, SPF_IS_XML | SPF_ASYNC, NULL);

}
 #ifdef _UNICODE

void CSpeech::say(const TCHAR p_tchar[])
{
	if ((!pVoice) || (b_muted)) return;


 pVoice->Speak(p_tchar, SPF_IS_XML | SPF_ASYNC, NULL);

}

void CSpeech::say(const WCHAR p_tchar[], DWORD dw_options)
{
    if ((!pVoice) || (b_muted)) return;
    
    pVoice->Speak(p_tchar, dw_options, NULL);
    
}


#endif

CSpeech::~CSpeech()
{
#ifdef _DEBUG
 //   LogMsg(_T("Killing speech system."));
#endif
    SAFE_RELEASE(pVoice);
    CoUninitialize();
    
}