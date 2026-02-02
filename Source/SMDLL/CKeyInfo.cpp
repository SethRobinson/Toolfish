#include "CKeyInfo.h"


CKeyInfo::CKeyInfo()
{

    //the i_crap is to fool the DATA_SEG into including all this
    memset(this, 0, sizeof(CKeyInfo));


                       
}


CKeyInfo::~CKeyInfo()
{
    ClearKeys();


}


void CKeyInfo::ClearKeys()
{
    //    SAFE_FREE(m_p_a_keys);
        m_i_size = 0;
}

int CKeyInfo::AddKey(byte vk_key, bool b_control, bool b_alt, bool b_shift, bool b_pasthrough, byte vk_replace)
{
    if (m_i_size >= C_MAX_KEYS)
    {
        //have too many assigned
        return -1; //error
    }
    m_i_size++;
    int i_index = m_i_size-1;
    //m_p_a_keys = (Key *)realloc(m_p_a_keys, m_i_size*sizeof(Key));
 
    m_p_a_keys[i_index].b_alt = b_alt;
    m_p_a_keys[i_index].vk_key = vk_key;
    m_p_a_keys[i_index].b_control = b_control;
    m_p_a_keys[i_index].b_passthrough = b_pasthrough;
    m_p_a_keys[i_index].b_shift = b_shift;
    m_p_a_keys[i_index].vk_replacement= vk_replace;

    return i_index;
}
