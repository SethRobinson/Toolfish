// Common Library header for DLL and application-- lib1.h
#ifdef SM_EXPORTS
#define LINKDLL __declspec( dllexport) extern 
#include "..\shared_funcs.h"
#else
#define LINKDLL __declspec( dllimport)
#include "..\shared_funcs.h"
#endif                                         
//struct FunctionTable;                                                        

/*
LINKDLL HHOOK h_hook;
LINKDLL HHOOK h_mouse_hook;                                        
                                               
LINKDLL HHOOK h_message_hook;                                                          
                                                                                  

LINKDLL void * (__stdcall *pMalloc)(long);


LINKDLL HWND	g_main_hwnd; //Main hinstance of dialog
LINKDLL HINSTANCE g_hModule	;  //our hinstance
  

*/

LINKDLL bool WINAPI install_hook(HINSTANCE dll_hinstance, HWND hinstance_app);
LINKDLL bool WINAPI kill_hooks(void);
LINKDLL void WINAPI SetupFunctionTable( void * (__stdcall *p_func)(long) );
LINKDLL void WINAPI AddKey(byte vk_key, bool b_control, bool b_alt, bool b_shift, bool b_pasthrough, byte vk_replace);
LINKDLL void WINAPI ClearKeys();
LINKDLL void WINAPI SetOverlay(bool b_on_off, int i_overlay);



			   