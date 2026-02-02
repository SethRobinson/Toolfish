
//do NOT change the overall byte size of this structure, it is saved inside of
//Actions and the config file and it would screw them up with loading/saving.
#pragma once
#ifndef _UNLOCK_UTILITY_
#include "stdafx.h"
#else
#include "unlock_util\\stdafx.h"
#endif


class CVoice
{
public:
    void SetSpeed(int i_speed){m_i_speed = i_speed;}
    int GetSpeed() {return m_i_speed;}
    void SetPitch(int i_pitch){m_i_pitch = i_pitch;}
    int GetPitch() {return m_i_pitch;}
  void SetVolume(int i_volume){m_i_volume = i_volume;}
    int GetVolume() {return m_i_volume;}
   
    void SetDefaults(); //I use this instead of a constructor, because some other
    //classes that use this do a ZeroMemory and would screw up the settings, so
    //I let them call this in their constructer.
    void Say(const TCHAR *p_words); //say it with this voice
    void SayTest(const TCHAR * p_words);

private:

    int m_i_pitch;
    int m_i_speed;
    int m_i_volume;
    unsigned char m_filler[32]; //reserve for future expansion

};
