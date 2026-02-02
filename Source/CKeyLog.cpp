#include "stdafx.h"
#include "CKeyLog.h"
CKeyLog g_key_log;


CKeyLog::CKeyLog()
{
   memset(this, 0, sizeof(CKeyLog));
}

CKeyLog::~CKeyLog()
{
    LogActive(false);
}

void CKeyLog::LogActive(bool b_active)
{
    
   if (b_active == true)
   {
       if (!m_b_log_active)
       {
           //log was off, turn it on
           	char st_final[255];
        	sprintf(st_final, "\r\nLogging started on %s at %s.\r\n", uni(show_date()).to_st(), uni(show_time()).to_st());
             m_b_log_active = b_active;
            //sprintf(st_final, "\r\nLog started.\r\n");
            WriteMessage(st_final);

       }
   }  else
   {

       if (m_b_log_active)
       {
           //turn it off
         	char st_final[255];
    	    //sprintf(st_final, "\r\nLogging ended on %s at %s.\r\n", show_date(), show_time());
sprintf(st_final, "\r\nLogging ended on %s at %s.\r\n", uni(show_date()).to_st(), uni(show_time()).to_st());
                         WriteMessage(st_final);
     }
 }
    
    m_b_log_active = b_active;

}


void CKeyLog::WriteMessage(char message[])
{
	if (m_b_log_active)
	{
	  add_text(message, m_st_key_log);
	}
}
void CKeyLog::SetFileName( char p_st_new[])
{
    strcpy(m_st_key_log, p_st_new);
}

void CKeyLog::FlushBuffer()
{
    if (m_i_buffer_pointer == 0) return; 
    FILE *fp;
     if ( (fp = fopen(m_st_key_log, "a")) == NULL)
	 {
	   //error opening/creating file
	 	 MessageBox(NULL, _T("Toolfish Error"), _T("Couldn't open file."), MB_OK);

	 } else
	 {
	  //write file
		 //let's find the last part of it..
	
		 fwrite(m_ch_buffer, 1, m_i_buffer_pointer, fp);
	     fclose(fp);
	 }

     //clear buffer
    ClearBuffer();

}

void CKeyLog::AddChar(char ch)
{
    if (!m_b_log_active) return;
 
    if (ch == 13) 
    {
      
        ch = '\n';
    }
 if (ch == 8)
 {
   //backspace
  	 if (m_i_buffer_pointer > 0)
	 m_i_buffer_pointer--;
     return;
 }
 m_ch_buffer[m_i_buffer_pointer] = ch;
 m_i_buffer_pointer++;
 if ( (m_i_buffer_pointer == C_KEY_LOG_BUFFER_SIZE) || (ch == '\n'))
 {
   //dump buffer to HD
   	 FlushBuffer();
 }
				
}



void CKeyLog::ClearBuffer()
{

 memset(m_ch_buffer, 0, sizeof(m_ch_buffer));
 m_i_buffer_pointer = 0;

}
