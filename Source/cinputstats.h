#pragma once

#ifndef _UNLOCK_UTILITY_
#include "stdafx.h"
#endif

enum
{
C_MOUSE_LEFT = 0,
C_MOUSE_RIGHT,
C_MOUSE_MIDDLE,
C_MOUSE_WHEEL,

C_MAX_MOUSE_BUTTONS = 5
};



class CInputStats
{
public:
    unsigned long GetKeys(){return m_ul_keyboard_taps;}
    CString GetMouseAsText();
    void AddKey(){m_ul_keyboard_taps++;}
    void AddMouse(unsigned long ul_new){m_ul_mouse_pixels += ul_new;}
    void Reset();
    void AddMouseButton(int i_button);
    unsigned long GetMouseButton(int i_button) {return m_a_mouse_clicks[i_button];}
    CString GetMouseButtonAsText(int i_button);

private:

   //don't change the size of this struct.  I was lazy and didn't create a smart loader for it
   //so size changes will screw up older versions, just add new data somewhere else or create
   //an updater.

   unsigned long m_ul_keyboard_taps; //how many times the keyboard has been tapped
   unsigned long m_ul_mouse_pixels; //how many times the mouse has been tapped
   unsigned long m_a_mouse_clicks[C_MAX_MOUSE_BUTTONS];
   
   byte buffer[30];

};

