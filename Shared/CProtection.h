#pragma once


#include "all.h"
#include "CEncrypt.h"



class CProtection
{
private:   
   char st_license[200];
   char m_st_name[80];
  
   float m_f_serial_div;
   bool m_b_serial_mod_active;
   int m_i_serial_mod;

    public:
   bool b_validated;
   bool is_validated();
   int i_max_players;
   int i_max_on;
   int m_i_master_seed;
   int m_i_seed;
   bool m_b_use_fixed_reduce; //i had a bug but I can't change it because FQ
   //servers out tehr eneed it for the reg code stuff

   CProtection();
   DWORD CProtection::get_serial();
   char * CProtection::make(DWORD dw_hd, int i_max_users, int i_max_on, const char *cst_name);
    void CProtection::demake(char *st_temp, char *p_st_name);
   void CProtection::decode_to_numbers(char *st_temp);
   void CProtection::demake_check();
   void CProtection::UseFixedReduce(bool b_new);
   bool CProtection::MakeUniqueStringFromSystemInfo(char *p_out, DWORD dw_mult);
   void SetSerialMod(float f_div, int i_mod);
   void Reset();

};



extern CProtection cls_pro;