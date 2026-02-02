#include "CProtection.h"


#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CProtection cls_pro;


CProtection::CProtection()
{
    st_license[0] = 0;
    b_validated = false;
	i_max_players = 0;
	i_max_on = 0;
    randomize();
    m_i_master_seed = random(100000);
    m_b_use_fixed_reduce = false;
    m_b_serial_mod_active = false;
    m_st_name[0] = 0;

}

void CProtection::Reset()
{

}

void CProtection::UseFixedReduce(bool b_new)
{
    m_b_use_fixed_reduce = b_new;
}

void CProtection::SetSerialMod(float f_div, int i_mod)
{
    m_f_serial_div = f_div;
    m_i_serial_mod = i_mod;

    m_b_serial_mod_active = true;

}


bool CProtection::is_validated()
{
  return b_validated;
}


void convert_numbers_to_letters(char p_cst[])
{
   	unsigned int i_length = strlen(p_cst);
	for (unsigned int i=0; i < i_length; i++)
	{
	 p_cst[i] += 21;
	}
}

void convert_letters_to_numbers(char p_cst[])
{
   	unsigned int i_length = strlen(p_cst);
	for (unsigned int i=0; i < i_length; i++)
	{
	 p_cst[i] -= 21;
	}
}


char move_char_caps_only(char c_in, int i_offset)
{
   while(i_offset != 0)
   {
	 if (i_offset > 0)
	 {
	   i_offset--;
	   c_in++;
	   if (c_in > 90)
		   c_in = 65;
	 }

	 if (i_offset < 0)
	 {
	   i_offset++;
	   c_in--;
	   if (c_in < 65) c_in = 90;

	 }
   }
  return c_in;
}



char * CProtection::make(DWORD dw_hd, int i_max_users, int i_max_on, const char *cst_name)
{
     	
    
    static char st_temp[100];


    #ifdef _INCLUDE_ENCODER
  	char st_name[100];
    int i_internal_checksum  = (((i_max_users + i_max_on)*3)-2);
	
	if (strlen(cst_name) > 50)
	{
	 LogMsg("Name of %s is illegal.",cst_name);
	 strcpy(st_temp, "Error");
     return st_temp;
	}
 
	strcpy(st_name, cst_name);
	
    CEncrypt e;
	unsigned int i_key = e.encrypt_piece((unsigned char*)st_name, strlen(st_name), 9);

	sprintf(st_temp,"%d,%d,%d", i_internal_checksum+i_key, (i_max_users*3)+9, (i_max_on*4)+5);
   // log_msg("Created key of %s.",st_temp);

	//encrypt the whole thing
	int i_r_key;
    
    if (m_b_use_fixed_reduce)
    {
    i_r_key = e.reduce_fixed(dw_hd) / 4;

    }   else
    {
    
    i_r_key = e.reduce(dw_hd) / 4;
    }
	convert_numbers_to_letters(st_temp);
	 //log_msg("In letters: %s.",st_temp);
//	 convert_letters_to_numbers(st_temp);
//	 log_msg("Back to numbers: %s.",st_temp);

	 //use DQ_HD reduce key + 5
	int i_length = strlen(st_temp);
	for (int i=0; i < i_length; i++)
	{
	 st_temp[i] = move_char_caps_only( st_temp[i], i_r_key+i);
	}
     //log_msg("Final: %s.",st_temp);
#endif
	 return st_temp;
}
void CProtection::decode_to_numbers(char *st_temp)
{
	
	CEncrypt e;
    int i_r_key;
    
    if (m_b_use_fixed_reduce)
    {
    i_r_key = e.reduce_fixed(get_serial()) / 4;

    }   else
    {
    
    i_r_key = e.reduce(get_serial()) / 4;
    }

  	int i_length = strlen(st_temp);
	for (int i=0; i < i_length; i++)
	{
	 st_temp[i] = move_char_caps_only( st_temp[i], -(i_r_key+i));
	}
	convert_letters_to_numbers(st_temp);
}

void CProtection::demake(char *p_st_temp, char *p_st_name)
{
 //	  #ifndef _INCLUDE_ENCODER
    char st_temp[200];
    strcpy(st_temp, p_st_temp);
    

	//decrypt everything
  	strcpy(st_license, p_st_temp);
    strcpy(m_st_name, p_st_name);

	decode_to_numbers(st_temp);
   //log_msg("Back to numbers: %s.",st_temp);
    
   char st_convert[100];
   //let's extract our numbers
   seperate_string(st_temp, 1, ',', (char*)&st_convert);
   int i_checksum = atol(st_convert);
   seperate_string(st_temp, 2, ',', (char*)&st_convert);
   int i_players = (atol(st_convert)-9)/3;
   seperate_string(st_temp, 3, ',', (char*)&st_convert);
   int i_on = (atol(st_convert)-5)/4;

   
   CEncrypt e;
   char st_encrypt_temp[100];
   strcpy(st_encrypt_temp, this->m_st_name);
   unsigned int i_key = e.encrypt_piece((unsigned char*)st_encrypt_temp, strlen(st_encrypt_temp), 9);
   m_i_seed = m_i_master_seed; //make sure this code bit got run

   //log_msg("Checksum is %d. Play is %d, on is %d.",i_checksum, i_players, i_on);
   if ( i_checksum != ((((i_players + i_on)*3)-2)+(int)i_key) )
   {
	 //bad code
	  	this->b_validated = false;
	this->i_max_on = 0;
	this->i_max_players = 0;


   }  else
   {
	this->b_validated = true;
	this->i_max_on = i_on;
	this->i_max_players = i_players;
   }
 
   i_checksum = 0;
   i_players = 0;
   i_key = 0;
   //clear memory to make it harder for hackers
   memset(&st_convert, 0, 100);

//#endif
}



void CProtection::demake_check()
{
 	  #ifndef _INCLUDE_ENCODER
	
	//decrypt everything
  	char st_temp[500];
	strcpy(st_temp, st_license);
	decode_to_numbers(st_temp);
   //log_msg("Back to numbers: %s.",st_temp);
    
   char st_convert[100];
   //let's extract our numbers
   seperate_string(st_temp, 1, ',', (char*)&st_convert);
   int i_checksum = atol(st_convert);
   seperate_string(st_temp, 2, ',', (char*)&st_convert);
   int i_players = (atol(st_convert)-9)/3;
   seperate_string(st_temp, 3, ',', (char*)&st_convert);
   int i_on = (atol(st_convert)-5)/4;

   
   CEncrypt e;
   char st_encrypt_temp[100];
   strcpy(st_encrypt_temp, this->m_st_name);
   unsigned int i_key = e.encrypt_piece((unsigned char*)st_encrypt_temp, strlen(st_encrypt_temp), 9);
   m_i_seed = m_i_master_seed; //make sure this code bit got run

   //log_msg("Checksum is %d. Play is %d, on is %d.",i_checksum, i_players, i_on);
   if ( (i_checksum != ((((i_players + i_on)*3)-2)+(int)i_key)) || (i_players != this->i_max_players) ||
	   (i_on != this->i_max_on))
   {
	 //um, this is just a bad code..
	  	this->b_validated = false;
	this->i_max_players = 0;


   }  else
   {
	   //good
   }

   i_checksum = 0;
   i_players = 0;
   i_key = 0;
   //clear memory to make it harder for hackers
   memset(&st_convert, 0, 100);

#endif
}

bool CProtection::MakeUniqueStringFromSystemInfo(char *p_out, DWORD dw_mult)
{
    DWORD dw_temp = get_serial();
    dw_temp *= dw_mult;
   //ok, let's convert the #'s to a string
    sprintf(p_out, "%d", dw_temp);
    convert_numbers_to_letters(p_out);
     return true; //no errors
}

DWORD CProtection::get_serial()
{
   	DWORD dwDiskSerial;
	if (!GetVolumeInformation(_T("C:\\"), NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL))
	{
	 return 0;
	}

    if (dwDiskSerial != 0)
    {
        if (m_b_serial_mod_active)
        {
            dwDiskSerial = unsigned long ((dwDiskSerial) /  unsigned long(m_f_serial_div));
            dwDiskSerial += m_i_serial_mod;
        }
    }
    
    return dwDiskSerial;
}
