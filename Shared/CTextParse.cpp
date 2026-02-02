/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */


const int C_LINE_SIZE = 2024;

#include "CTextParse.h"
#include "uni.h"
#include <cstdio>
#include <cassert>

CTextParse::CTextParse()
{
 b_unicode = false;   
 fp = NULL;
 st_path[0] = 0;
 b_strip_tab = true; //by default we will convert tab to spaces
 p_buffer = NULL; //we'll allocate this when we need it
 ui_cur_pos = 0; 
 ui_buffer_size = 0;
 p_word = (char*) malloc(2024*8);  //biggest possible word
 if (!p_word) LogError(_T("Out of memory in CTextParse"));

 p_word[0] = 0;
 m_i_line_buffer_size = C_LINE_SIZE*2; //this may be resized later if needed
 p_line = (char*) malloc(m_i_line_buffer_size);  //biggest possible word
 if (!p_line) LogError(_T("Out of memory in CTextParse"));

p_line[0] = 0;
}

void CTextParse::Close()
{
  if (fp) 
  {
    fclose(fp);
	fp = NULL;
  }
  
}

CTextParse::~CTextParse()
{
  Close();
  if (p_buffer)
  {
    free(p_buffer);
	p_buffer = NULL;

  }
   if (p_word)
  {
    free(p_word);
	p_word = NULL;

  }
   SAFE_FREE(p_line);

}

void CTextParse::set_path(char st_new_path[255])
{
  strcpy(st_path, st_new_path);
}

//returns NULL if it fails

char * CTextParse::GetBufferAtWord(char *p_search)
{
    //scan through each line and try to find it
    if (!p_buffer)
    {
        LogError(_T("CTextParse:  Can't GetBufferAtWord, buffer is null."));
        return false;
    }
  
    return strstr(p_buffer, p_search);
}




char * CTextParse::GetPointerToFirstLineStartingWith(char *p_search)
{
    //scan through each line and try to find it
    if (!p_buffer) return false;
    //reset the get line thingie
    int i_search_length = strlen(p_search);
    
    this->ResetGetNextLine();
    char *p_ln;
    while (get_next_line(&p_ln))
    {
        if (strncmp(p_ln, p_search, i_search_length) == 0)
        {
            return p_ln;
        }

    }
    return NULL;

}

bool CTextParse::Save(TCHAR *p_st_file)
{
   
#ifdef _UNICODE
    FILE *lfp = _wfopen(p_st_file, L"wb");

    if (!lfp)
    {
        LogError(_T("Unable to create file %s."), p_st_file);
        return false;
    }


#else
    FILE *lfp = fopen(p_st_file, "wb");

    if (!lfp)
    {
        LogError("Unable to create file %s.",p_st_file);
        return false;
    }


#endif
  
    //let's write out the whole thing
   if (p_buffer)
   {
       int i_size_of_char = sizeof(char);
       if (this->IsUnicode()) i_size_of_char = sizeof(WCHAR);

   fwrite(p_buffer,i_size_of_char, ui_buffer_size, lfp);
   } else
   {
       LogError(_T("CTextParse says there is nothing to save!"));
   }

   fclose(lfp);
   return true; //all ok
}

void CTextParse::CheckForUnicode()
{

   	if ( (unsigned short)p_buffer[0] == 65535)
    {
        //LogMsg("Unicode detected.");
        //	LogMsg(p_buffer);	 
        if (b_unicode == false)
        {
        
        b_unicode = true;
        ui_cur_pos = 1; //skip the unicode marker
        //ui_buffer_size /= 2;

        }
    }	   

}

unsigned long CTextParse::GetBuffByteSize()
{
    return ui_buffer_size;
};


bool CTextParse::open(const char st_fname[255])
{
   Close(); //if a file was already open, this will close it

   char st_name[255];
  sprintf(st_name, "%s%s",st_path, st_fname);
   //open new file
   fp=fopen(st_name, "rb");

   
	 if (!fp)
	 {
         //couldn't open/create file
#ifdef _UNICODE
         LogMsg(_T("Couldn't open %s.."),uni(st_name).us_data);

#else
         LogMsg("Couldn't open %s..",st_name);
#endif
	   return false;
	 }

   fseek(fp, 0, SEEK_END); 


   ui_buffer_size = ftell(fp);
   
 // LogMsg("Buffer size is %d.", ui_buffer_size);
   //move back to the beginning

   fseek(fp, 0, SEEK_SET);

   //make sure we have enough memory for it
   this->p_buffer = (char*) realloc(p_buffer, ui_buffer_size+2);

   //load the whole thing in
   fread(p_buffer, ui_buffer_size, 1, fp);
	b_unicode = false;
	ui_cur_pos = 0;
	//we got the two bytes we needed
    CheckForUnicode();
    p_buffer[ui_buffer_size] = 0;		 //force ending cr
	 p_buffer[ui_buffer_size+1] = 0;

	 ui_buffer_size += 2;

	
	 Close(); //if a file was already open, this will close it

	 
     //file has been opened
//	 flushall();         //fix super weird crash I was having?
     return true;

}

#ifndef _NO_ZLIB

bool CTextParse::OpenFromZip(CFileLib * p_lib, char *st_fname)
{
   Close(); //if a file was already open, this will close it
 
   //by telling it 0, it's saying malloc the buffer for us.  It will allocate it
   //2 bytes larger than we need btw, in case we need add a NULL.

    if (!p_lib->UnzipFileToMemory(st_fname, (void**)&p_buffer, 0, &ui_buffer_size))
    {
       // LogError("CTextParse couldn't get %s from the zip.", st_fname);
        return false;
    }
  
   b_unicode = false;
	ui_cur_pos = 0;
	//we got the two bytes we needed
	if ( (unsigned short)p_buffer[0] == 65535)
	{
			 //LogMsg("Unicode detected.");
	//	LogMsg(p_buffer);	 
		b_unicode = true;
	         ui_cur_pos = 1; //skip the unicode marker
	 }	   
	 p_buffer[ui_buffer_size] = 0;		 //force ending cr
	 p_buffer[ui_buffer_size+1] = 0;

	 ui_buffer_size += 2;

	 //retro fit for unicode functions:

	 if (b_unicode) ui_buffer_size /= 2;
     return true;

}
#endif

void CTextParse::ClearBuffer()
{

    if (p_buffer)
  {
    SAFE_FREE(p_buffer);
    ui_buffer_size = 0;
     ui_cur_pos = 0;
  }

}

void CTextParse::SetTextBuffer(char *pNewBuffer)
{
    ClearBuffer();
    p_buffer = pNewBuffer;
    ui_buffer_size = strlen(pNewBuffer);
}

//use this to add text manually, just to help you parse it.  When including size, make
//sure you include the NULL char too.
void CTextParse::AddText(const char *p_new, int i_size)
{
     p_buffer = (char*)realloc(p_buffer, ui_buffer_size+i_size); 
     if (!p_buffer)
     {
         LogError(_T("Realloc failed in CTextParse!"));
         return;
     }
     memcpy(p_buffer+ui_buffer_size, p_new, i_size);
	 ui_buffer_size += i_size;
	 return;
}

void CTextParse::ResetGetNextLine()
{
 	ui_cur_pos = 0; //start over
		if (b_unicode) ui_cur_pos = 2; //skip unicode mark in the beginning
	
}

bool CTextParse::get_next_line(char **pointer_to_buff)
{
    if (b_unicode)
    {
        //why are reading a unicode file as ANSI?  Oh well
        return get_next_line_wide((WCHAR**)pointer_to_buff);
    }
    p_line[0] = 0; //clear the line
	int i_position = 0;
	if (!p_buffer)
	{
	  LogMsg(_T("Can't get next line, no file was opened."));
	  return false;
	}

  
    if (ui_cur_pos >= this->ui_buffer_size)
	{
		//already read it, no more to get
		ui_cur_pos = 0; //start over
		if (b_unicode) ui_cur_pos = 2; //skip unicode mark in the beginning
		*pointer_to_buff = NULL;
        return false;
	}
   	*pointer_to_buff =  p_line;


	//get ready for the next line

   	if (b_unicode)
	{
		//it's unicode.  special handling is done to find the 'return'.			
		while (ui_cur_pos < ui_buffer_size)
			{
				//find the next break point
				if (p_buffer[ui_cur_pos] == '\n')
				{
					//add null
				 	p_line[i_position] = 0; //add a char
					ui_cur_pos++;
				  	i_position++;

					//add an extra do hicky
				  	p_line[i_position] = 0; //add a char
					ui_cur_pos++;
				  
					//convert from unicode
		//found it
					return true;
				}
			 	p_line[i_position] = p_buffer[ui_cur_pos]; //add a char
				ui_cur_pos++;
				i_position++;
			}
	

	} else
	{
			while (ui_cur_pos < ui_buffer_size)
			{
				//find the next break point
				if (p_buffer[ui_cur_pos] == '\n')
				{
				//add null
						ui_cur_pos++;
		 	            
                        //insert the null OVER the \n..unless it was a totally blank line
                        if (i_position > 0)
                        {
                            
                            if (p_line[i_position-1] == '\r')
                            {
                                p_line[i_position-1] = 0;
                            } else
                            {
                                p_line[i_position] = 0;
                            }
                        } else
                        {
                            
                            
                            p_line[i_position] = 0; //add a char
                        }
                        return true;
                }
                p_line[i_position] = p_buffer[ui_cur_pos]; //add a char
                ui_cur_pos++;
                i_position++;
                
                if (i_position >= m_i_line_buffer_size-2)
                {
                    //um, we got a problem..
                    LogError(_T("Line too big, let's realocate the buffer"));
                    m_i_line_buffer_size *= 2;
                    p_line = (char*)realloc(p_line, m_i_line_buffer_size);
                    if (!p_buffer)
                    {
                        LogError(_T("CTextParse:  Error doing realloc."));
                        return false;
                    }
                }
            }
    }
    
    
	//no more to read
	//we need special handling here too
	p_line[i_position] = 0; //add a char
	
	if (b_unicode)
	{
		//unicode needs 2 nulls	
		ui_cur_pos++;
		p_line[i_position] = 0; //add a char
	}
	return (true);	
}


void CTextParse::StripStartingSpacesOfNextLine()
{
    assert(p_line);
    strip_beginning_spaces(p_line);

}

bool CTextParse::get_next_line_wide(WCHAR **pointer_to_buff)
{
 	p_line[0] = 0; //clear the line
	int i_position = 0;
	if (!p_buffer)
	{
	  LogMsg(_T("Can't get next line, no file was opened."));
	  return false;
	}

	if (ui_cur_pos >= this->ui_buffer_size/2)
	{
		//already read it, no more to get
		ui_cur_pos = 1; //start over
		return false;
	}
	
	
	*pointer_to_buff =  (WCHAR*)p_line;	

	//get ready for the next line

   	if (!b_unicode)
	{
	  LogError(_T("File isn't unicode, can't process."));
	  return false;
	}
		while (ui_cur_pos < ui_buffer_size/2)
			{
				//find the next break point
				if ( ((WCHAR*)p_buffer)[ui_cur_pos] == '\n')
				{
					//add null
				 	((WCHAR*)p_line)[i_position-1] = 0; //add a char
					ui_cur_pos++;
					return true;
				}
			 	((WCHAR*)p_line)[i_position] = ((WCHAR*)p_buffer)[ui_cur_pos]; //add a char
			
				
				if (b_strip_tab)
				{
					if (p_line[i_position] == L'	')
					{
						//it's a tab.  conver it to a space.
						p_line[i_position] = L' ';
					}
				}
				ui_cur_pos++;
				i_position++;
			}
	

	
	//no more to read
    	((WCHAR*)p_line)[i_position] = 0; //add a char
	
	return (true);	
}

//1 based index, do not ask for word 0!!!

char * CTextParse::get_word(int i_word_num, char ch_sep)
{
    p_word[0] = NULL;
    separate_string(p_line, i_word_num, ch_sep, p_word);
    return p_word;
}


//some misc generic functions


float FuzzyCompareText(const char *p_const_old, const char *p_new, int i_sample_size, int i_old_size, int i_new_size)
{
  
    //this is a destructive edit, make a copy
    char *p_old = new char[i_old_size+1]; //extra for the null
    strcpy(p_old, p_const_old);
    
    char *p_st_sample = new char[i_sample_size+1]; //one extra for a null I want to add,
    //makes it eaasier to output debug strings with it
    p_st_sample[i_sample_size] = 0; //0 based some sample size is actually 1 more

  
    //now we're ready to do it.
    int i_next_sample;

    int i_correct = 0; //how many characters are the same
    char *p_found = NULL;
    for (int i_new_cur = 0; i_new_cur < i_new_size;)
    {
       i_next_sample = i_sample_size;
       
       if (i_new_cur+i_next_sample > i_new_size)
       {
           //last bite needs to be smaller.
           i_next_sample = i_new_size-i_new_cur;
           //let's null terminate it in advance, other the strstr will be wrong
           p_st_sample[i_next_sample] = 0;

       }

       //grab the sample

       strncpy(p_st_sample, &p_new[i_new_cur], i_next_sample);

       //inc pointer
       i_new_cur += i_next_sample;
      
       //now let's see if this sample exists in our buffer, if it does advance our correct
       //char acounter and blank out the string we found so it would be used again for
       //a repeat of the same string.

       if (p_found = strstr(p_old, p_st_sample))
       {
           //add to our score
           i_correct += i_next_sample;
           
           //blank it out
           _strnset(p_found, 19, i_next_sample); //19 is 'device control char' .. i picked it because it should never be in a normal text file.  Hope I'm right.
       }  else
       {
           //wow, couldn't find it.
         //  LogMsg("%s", p_st_sample);
       }

    }
   
  // LogMsg("Total score is %d out of %d.", i_correct,  i_new_size);
   
   
   
   //releate the mem we used
    SAFE_DELETE_ARRAY(p_old);
    SAFE_DELETE_ARRAY(p_st_sample);
   
    return float(i_correct)/float(i_new_size);
}


float GetPercentOfChangeBetweenTexts(char *p_old, char *p_new)
{
    //first compare their sizes.
    int i_old_size = strlen(p_old);
    int i_new_size = strlen(p_new);
    
    if ( (i_old_size == 0) || (i_new_size == 0))
    {
        LogError(_T("Source or dest is null.  Can't compare them."));
        return 0; //just say there is no change.
    }
    float f_size_ratio = 1; //1 means no change

    if (i_old_size < i_new_size)
    {
        f_size_ratio = float(i_old_size)/float(i_new_size);
    } else
    {
        f_size_ratio = float(i_new_size)/float(i_old_size);
    }

    //f_size_ratio now holds 1 if they are the same size, or 0.5 if they are 50% different in size.
// LogMsg("Size ratio is %d.", int(f_size_ratio*100));

   //now let's do the actual content check


   float f_content_changed = FuzzyCompareText(p_old, p_new, 5, i_old_size, i_new_size);

   if (f_size_ratio < f_content_changed)
   {
      f_content_changed = f_size_ratio;
   }

    return 1-f_content_changed; 
}

void CleanFileName(TCHAR *p_name)
{
    //replace all weird cahracters with an underscore so it can be made into a filename correctly.

    int i_length = _tcslen(p_name);

    for (int i=0; i < i_length; i++)
    {
        if ( 
             (p_name[i] < 31 )
          || (p_name[i] == 34 ) //"
          || (p_name[i] == 42 )
          || (p_name[i] == 47 )
          || (p_name[i] == '%' )


          || (p_name[i] > 126 ) 
          || (p_name[i] == 58 ) 
          || (p_name[i] == 92 ) 
          || (p_name[i] == 124 ) //|
          || ( (p_name[i] > 59 ) && (p_name[i] < 61))
          || ( (p_name[i] > 61 ) && (p_name[i] < 64))
          )
        {
            //change it
            p_name[i] = '_';
        }
    }

}


