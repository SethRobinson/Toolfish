#include "..\Shared\all.h"

const C_MAX_KEYS = 512;

struct Key
{
public:

    bool b_shift, b_alt, b_control;
    bool b_passthrough;
    byte vk_key;
    byte vk_replacement; //0 for none
};

              

class CKeyInfo
{
public:
     CKeyInfo();
     ~CKeyInfo();
      void ClearKeys();
      int AddKey(byte vk_key, bool b_control, bool b_alt, bool b_shift, bool b_pasthrough, byte vk_replace);
      int GetSize(){return m_i_size;}
      Key * GetKey(int i_index){return &m_p_a_keys[i_index];}
private:
    
    int m_i_size;
    Key m_p_a_keys[C_MAX_KEYS]; //array of keys

};