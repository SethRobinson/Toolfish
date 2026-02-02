#pragma once
#include "CGlobals.h"
#include "..\\..\\shared\\Winsock\\RawSocket.h"
#include "wininet.h"

class CServerInfo
{
public:
    CServerInfo();
    CServerInfo::~CServerInfo();

    
   bool CServerInfo::GetServerInfo(char p_st_server[], char p_st_file[]); 
   bool CServerInfo::GetAuthorization(char *p_st_code, char *p_st_key_out, char *p_message_out, bool b_return_good_on_bad_code);
   
   char m_st_url[80];
    char m_st_upgrade_text[256];
    char m_st_upgrade_url[80];
    int m_i_port;
    float m_f_version_latest;
    float m_f_version_required;
    bool m_b_ready; //true if winsock was initted
};


extern CServerInfo server_info;


UINT CheckForUpdate(LPVOID pVoid);
