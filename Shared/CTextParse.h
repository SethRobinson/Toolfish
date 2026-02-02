/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


//Define _NO_ZLIB to not require Seth's zlib library.

#pragma once


#include "all.h"

#ifndef _NO_ZLIB
#include "CFileLib\\CFileLib.h"
#endif

float GetPercentOfChangeBetweenTexts(char *p_old, char *p_new);
void CleanFileName(TCHAR *p_name);
float FuzzyCompareText(const char *p_const_old, const char *p_new, int i_sample_size, int i_old_size, int i_new_size);


class CTextParse
{
    
public:
    CTextParse(); //clear everything
    ~CTextParse(); //close file if it was opened
    void Close();
    void set_path(char st_new_path[255]);
    bool open(const char st_fname[255]);
    bool get_next_line(char **pointer_to_buff);
    void ResetGetNextLine();
    char * get_word(int i_word_num, char ch_sep = '|');
    bool get_next_line_wide(WCHAR **pointer_to_buff);
    bool IsUnicode(){return b_unicode;}
    void CheckForUnicode();
    bool Save(TCHAR *p_st_file);
#ifndef _NO_ZLIB
    bool OpenFromZip(CFileLib * p_lib, char *st_fname);
#endif
    void AddText(const char *p_new, int i_size);
    char * GetPointerToFirstLineStartingWith(char *p_search);
    char * GetBufferAtWord(char *p_search);
    unsigned long GetBuffByteSize();
    char * GetBuffer(){return p_buffer;} //might be unicode, you need to cast it yourself
    void ClearBuffer();
    void StripStartingSpacesOfNextLine(); //call after calling get_next_line;
    void SetTextBuffer(char *pNewBuffer); //give it a MALLOCED buffer to use, it will delete it itself

private:
    
    bool b_unicode;
    FILE *fp;
    char st_path[255];
    char *p_line;
    char *p_buffer;
    unsigned long ui_buffer_size;  //how big the file we loaded was
    unsigned long ui_cur_pos;	//current position in the buffer
    char *p_word;
    bool b_strip_tab;
    
    int m_i_line_buffer_size; 
    
};