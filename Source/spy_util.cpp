#include "stdafx.h"
#include "spy_util.h"

#include "ProcessEvents.h"
#include "CKeyLog.h"
#include "CEvent.h"
#include "CAction.h"
#include "CGLobals.h"
#include "ProcessFindUtils.h"
#include "..\\..\\shared\\std_all.h"

void SetupKeyboardOverlay()
{
	SetOverlay(glo.m_b_key_overlay_active, 0); //second part not used yet
}


void ProcessOpenedWindow(HWND h_win)
{
	if (glo.m_b_log_programs_run)
	{
		TCHAR wst_name[MAX_PATH];
		GetExeFromHwnd(h_win, wst_name);


		TCHAR wst_win_text[MAX_PATH];
		GetWindowText(h_win, wst_win_text, MAX_PATH);
		if (glo.m_st_key_log)
		{
			char st_temp[300];
			sprintf(st_temp, "%s at %s: Opened %s (%s)\r\n", uni(show_date()).to_st(), uni(show_time()).to_st(), uni(wst_name).to_st(), uni(wst_win_text).to_st());
			add_text(st_temp, uni(glo.m_st_key_log).to_st());

		}
	}
}





void ProcessGotKey(WPARAM wParam,  // virtual-key code
	LPARAM lParam)
{

	//don't forget to handle the hotkey if that's what it is
	ProcessHotkeyEvents(wParam);

	// handle numeric keypad
	if (wParam >= 0x60 && wParam <= 0x69) wParam = wParam - 0x30;

	if (wParam >= VK_PRIOR) if (wParam <= VK_HELP)
	{
		//ignore these
		return;
	}

	if (wParam >= VK_F1) if (wParam <= VK_F24)
	{
		return;
	}

	switch (wParam)
	{

	case VK_LBUTTON:
	case VK_RBUTTON:
	case VK_CANCEL:
	case VK_MBUTTON:
	case VK_CONTROL:
	case VK_APPS:
	case VK_LMENU:
	case VK_RMENU:
	case VK_MENU:
	case VK_SHIFT:
	{
		break;
	}
	case VK_CLEAR:
	{
		break;
	}
	case VK_CAPITAL:
	{
		break;
	}

	case VK_TAB:
	{
		break;
	}
	default:
	{
		short nKeyState;
		int capsOn;
		int shiftOn;

		// caps lock on?
		nKeyState = GetAsyncKeyState(VK_CAPITAL);
		if (LOBYTE(nKeyState))
		{


			capsOn = 1;
		}
		else
		{
			capsOn = 0;
		}

		// shift key pressed?
		/*
		if (GetKeyState(VK_SHIFT) < 0)
		{
			 shiftOn = 1;
		}
		else
		{
		 shiftOn = 0;
		}
	  */

		shiftOn = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? true : false;



		// get the real key character
		char list = MapVirtualKey(wParam, 2);

		// handle shift key & caps lock
		if ((shiftOn == 1 && capsOn == 0) || (shiftOn == 0 && capsOn == 1))
		{
			list = toupper(list);
		}
		else
		{
			list = tolower(list);
		}

		// write it
		if (shiftOn == 1)
		{
			switch (list)
			{
			case '0':
			{
				list = ')';
				break;
			}
			case '1':
			{
				list = '!';
				break;
			}
			case '2':
			{
				list = '@';
				break;
			}
			case '3':
			{
				list = '#';
				break;
			}
			case '4':
			{
				list = '$';
				break;
			}
			case '5':
			{
				list = '%';
				break;
			}
			case '6':
			{
				list = '^';
				break;
			}
			case '7':
			{
				list = '&';
				break;
			}
			case '8':
			{
				list = '*';
				break;
			}
			case '9':
			{
				list = '(';
				break;
			}
			case '/':
			{
				list = '?';
				break;
			}
			case '`':
			{
				list = '~';
				break;
			}
			case '-':
			{
				list = '_';
				break;
			}
			case '=':
			{
				list = '+';
				break;
			}
			case '[':
			{
				list = '{';
				break;
			}
			case ']':
			{
				list = '}';
				break;
			}
			case ';':
			{
				list = ':';
				break;
			}
			case '\'':
			{
				list = '\"';
				break;
			}
			case '\\':
			{
				list = '|';
				break;
			}
			case ',':
			{
				list = '<';
				break;
			}
			case '.':
			{
				list = '>';
				break;
			}
			}
		}
		//list++;

		g_key_log.AddChar(list);
	}
	// flush buffer

	}

}



const int C_MAX_TO_SEND = 250;


INPUT ia[2000 * 4]; // The most we can use


void InjectString(TCHAR* str, int len)
{


	bool is_console = false;


	while (len > C_MAX_TO_SEND) // break it up
	{
		InjectString(str, C_MAX_TO_SEND);
		len -= C_MAX_TO_SEND;
		str += C_MAX_TO_SEND;
	}


	static WORD shiftscan = MapVirtualKey(VK_SHIFT, 0);
	static WORD menuscan = MapVirtualKey(VK_MENU, 0);
	static WORD controlscan = MapVirtualKey(VK_CONTROL, 0);

	ZeroMemory(ia, sizeof(ia));

	int i, j = 0;

	bool b_control = false;
	bool b_menu = false; //alt key
	bool b_shift = false;

	if (GetAsyncKeyState(VK_SHIFT)) b_shift = true;
	if (GetAsyncKeyState(VK_MENU)) b_menu = true;
	if (GetAsyncKeyState(VK_CONTROL)) b_control = true;

	if (b_shift)
	{

		//first turn any special keys off or else
		ia[j].type = INPUT_KEYBOARD;
		ia[j].ki.wVk = VK_SHIFT;
		ia[j].ki.wScan = shiftscan;
		ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

		j++;
	}
	if (b_menu)
	{

		ia[j].type = INPUT_KEYBOARD;
		ia[j].ki.wVk = VK_MENU;
		ia[j].ki.wScan = MapVirtualKey(VK_MENU, 0);
		ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

		j++;

	}

	if (b_control)
	{

		ia[j].type = INPUT_KEYBOARD;
		ia[j].ki.wVk = VK_CONTROL;
		ia[j].ki.wScan = MapVirtualKey(VK_CONTROL, 0);
		ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

		j++;
	}


	for (i = 0; i < len; i++)
	{

		/*
		 if (i < len-1)
		 if (str[i] == ' ' && str[i]+1 == ' ')
		 {
				//skip one of the extra spaces
				 continue;
		 }

		 if (str[i] == '\t')
		 {
			 //i++; //and skip one more
		 continue;
		 }
		 */

		if (str[i] == '\r')
		{
			ia[j].type = INPUT_KEYBOARD;
			ia[j].ki.wVk = VK_RETURN;
			ia[j].ki.wScan = MapVirtualKey(VK_RETURN, 0);
			ia[j].ki.dwFlags = 0;  // Important: clear flags for non-Unicode input
			j++;

			ia[j].type = INPUT_KEYBOARD;
			ia[j].ki.wVk = VK_RETURN;
			ia[j].ki.wScan = MapVirtualKey(VK_RETURN, 0);
			ia[j].ki.dwFlags = KEYEVENTF_KEYUP;
			j++;
			continue;
		}

		if (str[i] == '\n')
		{
			//i++;
			//ignore this completely
			continue;
		}

#ifdef _UNICODE
		// Unicode char -> VK_PACKET down only for console
		ia[j].type = INPUT_KEYBOARD;
		ia[j].ki.wVk = 0;                         // required for KEYEVENTF_UNICODE
		ia[j].ki.wScan = str[i];                  // UTF-16 code unit
		ia[j].ki.dwFlags = KEYEVENTF_UNICODE;     // 0x0004
		j++;

		if (!is_console)
		{
			ia[j].type = INPUT_KEYBOARD;
			ia[j].ki.wVk = 0;
			ia[j].ki.wScan = str[i];
			ia[j].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
			j++;
		}
#else

		DWORD temp_vk = VkKeyScan(str[i]);
		BOOL b_shift = false;

		b_shift = !!(temp_vk & 0x100);


		if (b_shift)
		{
			//shift key is done, add an event for that..

			ia[j].type = INPUT_KEYBOARD;
			ia[j].ki.wVk = VK_SHIFT;
			ia[j].ki.wScan = shiftscan;
			// ia[j].ki.dwFlags = KEYEVENTF_KEYUP;
			j++;

		}

		//ANSI version, not as cool but works
		ia[j].type = INPUT_KEYBOARD;
		ia[j].ki.wVk = VkKeyScan(str[i]);
		//ia[j].ki.wScan = str[i];

	   //   ia[j].ki.dwFlags = 0x0003; //scancode only, couldn't find the window define
		j++;

		// Key up
		ia[j].type = INPUT_KEYBOARD;
		ia[j].ki.wVk = ia[j - 1].ki.wVk;
		// ia[j].ki.wScan = ia[j - 1].ki.wScan;
		ia[j].ki.dwFlags = KEYEVENTF_KEYUP; //0x0003|
		j++;

		if (b_shift)
		{

			ia[j].type = INPUT_KEYBOARD;
			ia[j].ki.wVk = VK_SHIFT;
			ia[j].ki.wScan = shiftscan;
			ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

			j++;

		}

#endif



	}

	//optionally turn back on the control keys we had disabled
   /*
	if (b_shift)
   {

	//first turn any special keys off or else
	  ia[j].type = INPUT_KEYBOARD;
	  ia[j].ki.wVk = VK_SHIFT;
	  ia[j].ki.wScan = shiftscan;
	 // ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

	  j++;
   }
   if (b_menu)
   {

	  ia[j].type = INPUT_KEYBOARD;
	  ia[j].ki.wVk = VK_MENU;
	  ia[j].ki.wScan = MapVirtualKey(VK_MENU, 0);
	 // ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

	  j++;

   }

   if (b_control)
   {

	  ia[j].type = INPUT_KEYBOARD;
	  ia[j].ki.wVk = VK_CONTROL;
	  ia[j].ki.wScan = MapVirtualKey(VK_CONTROL, 0);
	 // ia[j].ki.dwFlags = KEYEVENTF_KEYUP;

	  j++;

   }
	  */
	

	  //delay between each key
	//LogMsg("Injecting %d characters", j);
	for (int i = 0; i < j; i++)
	{
		SendInput(1, &ia[i], sizeof(INPUT));
		Sleep(1);
		
	}


}

void ReplaceStrings(TCHAR* p_dest, const TCHAR* p_src, int i_size_of_dest, int i_size_of_src, CEvent* p_event)
{

	int i_dest_index = 0; //start at 0
	for (int i = 0; i < i_size_of_src + 1; i++) //the added one is for the final null
	{

		if (p_src[i] == '$')
		{
			//this may or may not be a control sequence
			const TCHAR* p_ws_ip = _T("IP_ADDRESS$");

			if (_tcsnicmp(p_src + i + 1, p_ws_ip, _tcslen(p_ws_ip)) == 0)
			{
				_tcscpy(&p_dest[i_dest_index], uni(app_glo.GetIP()).GetAuto());

				//fast forward to the NULL
				for (; p_dest[++i_dest_index];);
				i += _tcslen(p_ws_ip);
				continue;
			}

			//this may or may not be a control sequence
			const TCHAR* p_ws_time = _T("TIME$");

			if (_tcsnicmp(p_src + i + 1, p_ws_time, _tcslen(p_ws_time)) == 0)
			{
				_tcscpy(&p_dest[i_dest_index], show_time());
				//fast forward to the NULL
				for (; p_dest[++i_dest_index];);
				i += _tcslen(p_ws_time);
				continue;
			}


			const TCHAR* p_ws_clip = _T("CLIPBOARD$");

			if (_tcsnicmp(p_src + i + 1, p_ws_clip, _tcslen(p_ws_clip)) == 0)
			{
				//insert clipboard contents
				OpenClipboard(app_glo.GetHWND());
				if (!IsClipboardFormatAvailable(CF_TEXT))
				{
					LogMsg(_T("No text on clipboard, ignoring $CLIPBOARD$ token."));
					p_dest[i_dest_index] = 0;
				}
				else
				{
#ifdef _UNICODE
					HANDLE hData = GetClipboardData(CF_UNICODETEXT);
#else
					HANDLE hData = GetClipboardData(CF_TEXT);
#endif

					const int C_MAX_CLIPBOARD_SIZE = 1900;
					TCHAR buff[C_MAX_CLIPBOARD_SIZE + 1];
					memset(buff, 0, (C_MAX_CLIPBOARD_SIZE + 1) * sizeof(TCHAR));

					if (_tcslen((TCHAR*)hData) > C_MAX_CLIPBOARD_SIZE)
					{
						LogMsg(_T("Clipboard has a large amount of text on it, ignoring $CLIPBOARD$ as a safety precaution."));
						p_dest[i_dest_index] = 0;
					}
					else
					{
						_tcscpy(buff, (TCHAR*)hData);
						//skip the URL if we need to
						_tcscpy(&p_dest[i_dest_index], buff);
					}


					//fast forward to the NULL

					if (p_dest[i_dest_index] != 0)
					{
						for (; p_dest[++i_dest_index];);
					}

					GlobalUnlock(hData);
				}

				CloseClipboard();
				i += _tcslen(p_ws_clip);
				continue;
			}


			const TCHAR* p_ws_clipURL = _T("CLIPBOARD_URL$");

			if (_tcsnicmp(p_src + i + 1, p_ws_clipURL, _tcslen(p_ws_clipURL)) == 0)
			{
				//insert clipboard contents
				OpenClipboard(app_glo.GetHWND());
				if (!IsClipboardFormatAvailable(CF_TEXT))
				{
					LogMsg(_T("No text on clipboard, ignoring $CLIPBOARD_URL$ token."));
					p_dest[i_dest_index] = 0;
				}
				else
				{
#ifdef _UNICODE
					HANDLE hData = GetClipboardData(CF_UNICODETEXT);
#else
					HANDLE hData = GetClipboardData(CF_TEXT);
#endif

					const int C_MAX_CLIPBOARD_SIZE = 1900;
					TCHAR buff[C_MAX_CLIPBOARD_SIZE + 1];
					memset(buff, 0, (C_MAX_CLIPBOARD_SIZE + 1) * sizeof(TCHAR));

					if (_tcslen((TCHAR*)hData) > C_MAX_CLIPBOARD_SIZE)
					{
						LogMsg(_T("Clipboard has a large amount of text on it, ignoring $CLIPBOARD$ as a safety precaution."));
						p_dest[i_dest_index] = 0;
					}
					else
					{

						_tcscpy(buff, (TCHAR*)hData);
						//skip the URL if we need to
						WCHAR* pFinalText = buff;

						WCHAR* pPos = StrStrNIW(buff, L"http://", _tcslen(buff));
						if (pPos)
						{
							pFinalText = pPos + _tcslen(L"http://");
							//LogMsg("We foud it");
						}

						pPos = StrStrNIW(buff, L"https://", _tcslen(buff));
						if (pPos)
						{
							pFinalText = pPos + _tcslen(L"https://");
							//LogMsg("We foud it");
						}

						_tcscpy(&p_dest[i_dest_index], pFinalText);
					}


					//fast forward to the NULL

					if (p_dest[i_dest_index] != 0)
					{
						for (; p_dest[++i_dest_index];);
					}

					GlobalUnlock(hData);
				}

				CloseClipboard();
				i += _tcslen(p_ws_clipURL);
				continue;
			}



			const TCHAR* p_ws_email = _T("LAST_NEW_EMAIL_COUNT$");

			if (_tcsnicmp(p_src + i + 1, p_ws_email, _tcslen(p_ws_email)) == 0)
			{
				TCHAR tc_buffer[20];
				_ltot(app_glo.GetLastNewEmailCount(), tc_buffer, 10);
				_tcscpy(&p_dest[i_dest_index], tc_buffer);
				//fast forward to the NULL
				for (; p_dest[++i_dest_index];);
				i += _tcslen(p_ws_email);
				continue;
			}

			//this may or may not be a control sequence
			const TCHAR* p_ws_date = _T("DATE$");

			if (_tcsnicmp(p_src + i + 1, p_ws_date, _tcslen(p_ws_date)) == 0)
			{
				_tcscpy(&p_dest[i_dest_index], show_date());
				//fast forward to the NULL
				for (; p_dest[++i_dest_index];);
				i += _tcslen(p_ws_date);
				continue;
			}
			const TCHAR* p_ws_short_date = _T("SHORT_DATE$");

			if (_tcsnicmp(p_src + i + 1, p_ws_short_date, _tcslen(p_ws_short_date)) == 0)
			{
				_tcscpy(&p_dest[i_dest_index], show_small_date());
				//fast forward to the NULL
				for (; p_dest[++i_dest_index];);
				i += _tcslen(p_ws_short_date);
				continue;
			}


			//this may or may not be a control sequence
			const TCHAR* p_ws_url = _T("LAST_COMPARE_URL$");

			if (_tcsnicmp(p_src + i + 1, p_ws_url, _tcslen(p_ws_url)) == 0)
			{

				//ok, if the p_event we got isn't null, use it to search for a URL.  because things are
				//multithreaded the LAST_URL may be wrong without doing it this way.

				if (p_event)
				{
					CAction* p_action;
					if (p_action = p_event->FindAction(C_ACTION_COMPARE))
					{
#ifdef _UNICODE
						_tcscpy(&p_dest[i_dest_index], p_action->GetFilename());
#else
						_tcscpy(&p_dest[i_dest_index], uni(p_action->GetFilename()).to_st());

#endif

					}
					else
					{
						//default
						_tcscpy(&p_dest[i_dest_index], app_glo.GetLastURLCompared());

					}

				}
				else
				{
					_tcscpy(&p_dest[i_dest_index], app_glo.GetLastURLCompared());
				}


				//fast forward to the NULL
				for (; p_dest[++i_dest_index];);
				i += _tcslen(p_ws_url);
				continue;
			}


		}

		p_dest[i_dest_index++] = p_src[i];
	}

}







