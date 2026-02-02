/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

//defines

#pragma once
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afx.h>         // MFC core and standard components

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
//#include <afxdisp.h>        // MFC Automation classes
//#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h> //for CListCtrl


#define d_to_window false
#define d_to_var true

#define D_TO_WINDOW false
#define D_TO_VAR true


#ifndef SAFE_DELETE_OBJECT
#define SAFE_DELETE_OBJECT(p)      { if(p) { DeleteObject (p); (p)=NULL; } }
#endif
 
#include "all.h"
CString EncodeTextBase64(const CString& sText);

int set_selection_by_data(CComboBox * m_list, int i_sel);
int set_selection_by_data(CListBox * m_list, int i_sel);
int set_selection_by_data(CListCtrl * m_list, int i_sel);
CString int_to_cash(int i_amount);
CString float_to_cash(float i_amount);
CString ulong_to_cash(unsigned long i_amount);

bool AddText(CString cst_message, CString cst_filename);
CHARFORMATA CharFormatWToA(CHARFORMATW input);
CHARFORMATW CharFormatAToW(CHARFORMATA input);
bool load_file_into_cstring(CString cst_file, CString *cst_text);

#ifdef _UNICODE

#include "mfc_all_u.h"
#else

CString save_txt_file_dialog(CString cst_default);
CString load_txt_file_dialog(CString cst_default);
FILE * open_file_cst(CString file);
bool get_line_from_file_cst(FILE * fp, CString * dest);
bool separate_string_mfc (CString cst_input, int num, char liney, CString *cst_return);
bool force_range(int * i_original, int i_min, int i_max, CString cst_context);
bool fforce_range(float * i_original, float i_min, float i_max, CString cst_context);

bool exist_cst(CString name);


#endif

//** both

double get_time_difference_in_text(CString *cst_temp, time_t t_last_packet);
double get_time_difference_in_text_long(CString *cst_temp, time_t t_last_packet);

