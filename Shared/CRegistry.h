#pragma once

#include "windows.h"
#include "shellapi.h"

class CRegistry
{
	char st_reg_key[500];

public:

	CRegistry(char st_reg_key_new[], HKEY access = HKEY_CURRENT_USER);
	bool CRegistry::read_key_string(char st_key[], char *p_st_key);
    bool CRegistry::read_key_dword(char st_key[], DWORD *p_dw_key);
    bool CRegistry::CreateKey(char st_key[]);
    bool CRegistry::WriteDWORD(char st_key[], DWORD *p_dw_key);

private:
    HKEY m_hkey_access;
};