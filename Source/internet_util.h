#pragma once
#include "CGlobals.h"
#include "..\Shared\winsock\winsock_util.h"
#include "CEvent.h"
#include "CEventList.h" 
#include "..\Shared\CTextParse.h"


class CWebHeader
{


};

bool GetWebFile(const char * p_st_server, const char *p_st_file, CTextParse *p_parse, CWebHeader *p_header, const bool b_authenticate = false, const WCHAR *p_wst_logon = NULL, const WCHAR *p_wst_pass = NULL, bool b_https = false);
bool ProcessCompare( CEvent *p_event, int i_index);
bool BreakURLDown(const TCHAR *p_original_url, TCHAR *p_full, TCHAR * p_ws_domain, TCHAR * p_ws_path,
                  TCHAR * p_ws_filename, bool *p_b_https = NULL);
