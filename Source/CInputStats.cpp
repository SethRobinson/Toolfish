#include "CInputStats.h"
#include "..\Shared\all.h"
#include "..\Shared\mfc_all.h"

const int C_MOUSEPAD_BASE = 1000; //higher number means less distance from mouse movement

CString CInputStats::GetMouseAsText()
{
  CString st_temp;
  
 unsigned long ul_inches = m_ul_mouse_pixels / 1000;
 unsigned long ul_miles =  (ul_inches/12)/5280;

  ul_inches -= ul_miles*12*5280;

 //now extract feet
 unsigned long ul_feet = ul_inches / 12;

 ul_inches -= ul_feet * 12; 
 
 
 st_temp.Format(_T("%s miles, %s feet and %s inches"), ulong_to_cash(ul_miles),
     ulong_to_cash(ul_feet), ulong_to_cash(ul_inches));
  return st_temp;
}


void CInputStats::Reset()
{
    m_ul_mouse_pixels = 0;
    m_ul_keyboard_taps = 0;

    for (int i=0; i < C_MAX_MOUSE_BUTTONS; i++)
    {
        m_a_mouse_clicks[i] = 0;
    }

}    

void CInputStats::AddMouseButton(int i_button)
{
    m_a_mouse_clicks[i_button]++;
}

CString CInputStats::GetMouseButtonAsText(int i_button)
{
    CString cst_temp;
    cst_temp.Format(_T("%s"), ulong_to_cash(m_a_mouse_clicks[i_button]));
    return cst_temp;
}
