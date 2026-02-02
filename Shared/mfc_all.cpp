/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */

#include "mfc_all.h"
#include "uni.h"

#ifndef _UNICODE

//this file assumes you have a "LogMsg" procedure somewhere, if you don't
//just copy this one into YOUR code:
/*
void LogMsg(LPCSTR lpFormat, ...)
{
	va_list Marker;
	char szBuf[4048];
	
	// Write text to string
	// and append to edit control
	va_start(Marker, lpFormat);
	vsprintf(szBuf, lpFormat, Marker);
	va_end(Marker);
	CString cst_final;
	cst_final.Format("%s %s:%s",show_small_date(),show_time(),szBuf);
	OutputDebugString(cst_final);

//	g_dlg_main->Log(cst_final);
}
*/

//Don't send this a pointer a char array smaller than 256 or prepare for the worst.
  
//changes a number to be in a range of required.  If it is, it returns true.
bool force_range(int * i_original, int i_min, int i_max, CString cst_context)
{
 if (in_range(*i_original, i_min, i_max)) return false;
#ifdef _DEBUG
 LogMsg("force_range activated - var %s was %d.  Put in range.",cst_context,*i_original);
#endif 
 if (*i_original < i_min) *i_original = i_min;
 if (*i_original > i_max) *i_original = i_max;

 return false; //changed number

}

bool fforce_range(float * i_original, float i_min, float i_max, CString cst_context)
{
 if (in_range_float(*i_original, i_min, i_max)) return false;
#ifdef _DEBUG
 LogMsg("force_range activated - var %s was %d.  Put in range.",cst_context,int(*i_original));
#endif 
 if (*i_original < i_min) *i_original = i_min;
 if (*i_original > i_max) *i_original = i_max;

 return false; //changed number
}


bool separate_string_mfc (CString cst_input, int num, char liney, CString *cst_return) 
{
	//it's an old routine, I don't want to rewrite, I just wanted to use
	//cstring passing
	char return1[800];
	char str[1000];
	if (cst_input.GetLength() > 999) 
	{
	 LogMsg("Avoided a crash - separate_string_cst got a too long line");
	 return false;
	}
	strcpy(str,cst_input); 
	
	int l;
	unsigned int k;
	
	l = 1;
	strcpy(return1 ,"");
	
	for (k = 0; k <= strlen(str); k++)
	{
		
		if (str[k] == liney)
		{
			l++;
			if (l == num+1)
				goto done;
			
			if (k < strlen(str)) strcpy(return1,"");
		}
		if (str[k] != liney)
			sprintf(return1, "%s%c",return1 ,str[k]);
	}
	if (l < num)  strcpy(return1,"");
	
	replace("\n","",return1); //Take the /n off it.
	cst_return->Format("%s", return1);
	
	return(false);
	
done:
	
	if (l < num)  strcpy(return1,"");
	
	replace("\n","",return1); //Take the /n off it.
	cst_return->Format("%s", return1);
	//Msg("Took %s and turned it to %s.",str, return1);
	return(true);
}



//these are used to launch a url.  call like this:

//  winall_create_url_file("www.rtsoft.com/smartmute/chatwall");
//  int result = (int)ShellExecute(NULL,"open","temp.url", NULL,NULL, SW_MAXIMIZE);
//  ProcessError(this->m_hWnd,result, "smcw.url","www.rtsoft.com/smartmute/chatwall"); //show an error message if we couldn't open this
  
   CString load_txt_file_dialog(CString cst_default)
{
/*
       char command_line[400];
	char my_path[400];

	//strcpy(command_line, GetCommandLine());
	getdir(command_line, my_path);
  */
       
	 CFileDialog cfd(true, //true if open, false if close
		 ".TXT", //default file extension
		 cst_default, OFN_HIDEREADONLY,
		 "Text files (*.txt)|*.txt|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);

		 if(cfd.DoModal() == IDOK)
		 {
			return cfd.GetPathName();
		 } else

    return "";
}



  CString save_txt_file_dialog(CString cst_default)
{
/*
      char command_line[400];
	char my_path[400];

	//strcpy(command_line, GetCommandLine());
	getdir(command_line, my_path);
  */
      
	 CFileDialog cfd(false, //true if open, false if close
		 ".TXT", //default file extension
		 cst_default, OFN_HIDEREADONLY,
		 "Text files (*.txt)|*.txt|All files (*.*)|*.*|",//file filter
		 AfxGetApp()->m_pMainWnd);

		 if(cfd.DoModal() == IDOK)
		 {
			return cfd.GetPathName();
		 } else

    return "";
}

 FILE * open_file_cst(CString file)
{

	return(fopen( file, "r" ));

}

//get next line from a stream (opened with above)

bool get_line_from_file_cst(FILE * fp, CString * dest)
{
    char buff[256];
	buff[0] = 0; 	
   if( fgets( buff, 255, fp ) == NULL)
	{
		dest->Format("");
		return(false);
	}
			

	dest->Format("%s",buff);
	return true;
}

/* does file exist? */
bool exist_cst(CString name)
{
	FILE *fp;
	fp = fopen(name, "rb");
	if (!fp)
	{
		//	  fclose(fp);
		return(false);
	}
	
	fclose(fp);
	return(true);
}


bool load_file_into_cstring(CString cst_file, CString *cst_text)
{

  CString cst_temp;
//does the file exist?
  if (!exist_cst(cst_file)) return false;

  FILE *fp = open_file_cst(cst_file);

  if (!fp)
  {
	  LogError("Error opening file to load into string.");
	  return false;
  }

  while (get_line_from_file_cst(fp, &cst_temp))
  {
    *cst_text += cst_temp;
  }

  fclose(fp);

  return true;

}

#endif
 


// ********************** private
// *                    *
// *  EncodeTextBase64  *
// *                    *
// **********************
// Function: Returns the Base64-encoded version of a text string.
      
CString EncodeTextBase64(const CString& sText)
{
	uni u; //used to convert from unicode to char

    unsigned char cChar[255];
	int nIndex1, nIndex2, nIndex3, nIndex4;
	int nChars;
	CString sBase64 = "";
	char cTable[64 + 1];
    char *st_temp = new char[sText.GetLength()+10];
    if (!st_temp)
    {
        return _T("");
    }
	cTable[0] = 'A';
	cTable[1] = 'B';
	cTable[2] = 'C';
	cTable[3] = 'D';
	cTable[4] = 'E';
	cTable[5] = 'F';
	cTable[6] = 'G';
	cTable[7] = 'H';
	cTable[8] = 'I';
	cTable[9] = 'J';
	cTable[10] = 'K';
	cTable[11] = 'L';
	cTable[12] = 'M';
	cTable[13] = 'N';
	cTable[14] = 'O';
	cTable[15] = 'P';

	cTable[16] = 'Q';
	cTable[17] = 'R';
	cTable[18] = 'S';
	cTable[19] = 'T';
	cTable[20] = 'U';
	cTable[21] = 'V';
	cTable[22] = 'W';
	cTable[23] = 'X';
	cTable[24] = 'Y';
	cTable[25] = 'Z';
	cTable[26] = 'a';
	cTable[27] = 'b';
	cTable[28] = 'c';
	cTable[29] = 'd';
	cTable[30] = 'e';
	cTable[31] = 'f';

	cTable[32] = 'g';
	cTable[33] = 'h';
	cTable[34] = 'i';
	cTable[35] = 'j';
	cTable[36] = 'k';
	cTable[37] = 'l';
	cTable[38] = 'm';
	cTable[39] = 'n';
	cTable[40] = 'o';
	cTable[41] = 'p';
	cTable[42] = 'q';
	cTable[43] = 'r';
	cTable[44] = 's';
	cTable[45] = 't';
	cTable[46] = 'u';
	cTable[47] = 'v';

	cTable[48] = 'w';
	cTable[49] = 'x';
	cTable[50] = 'y';
	cTable[51] = 'z';
	cTable[52] = '0';
	cTable[53] = '1';
	cTable[54] = '2';
	cTable[55] = '3';
	cTable[56] = '4';
	cTable[57] = '5';
	cTable[58] = '6';
	cTable[59] = '7';
	cTable[60] = '8';
	cTable[61] = '9';
	cTable[62] = '+';
	cTable[63] = '/';

	cTable[64] = '=';

	nChars = sText.GetLength();
	int nPos;
	
	for (nPos = 0; nPos < nChars; nPos++) 
	{
	    //this is a low of work to convert a single unicode char to an ansi char, but do you know of a 
        //better way?  thought not!
        
        TCHAR temp_char[2];
        temp_char[0] = sText.GetAt(nPos);
        temp_char[1] = 0; //add null at the end, now ready for conversion
        
        cChar[nPos] = uni(temp_char).st_data[0]; 

	} // End for

	//   cChar[nPos]    cChar[nPos+1]   cChar[nPos+2]
	//        |               |               |
	// -------+------- -------+------- -------+-------
	// 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0 7 6 5 4 3 2 1 0
	// | | | | | | | | | | | | | | | | | | | | | | | |
	// x x x x x x x x x x x x x x x x x x x x x x x x
	// | | | | | | | | | | | | | | | | | | | | | | | |
	// 5 4 3 2 1 0 5 4 3 2 1 0 5 4 3 2 1 0 5 4 3 2 1 0
	// -----+----- -----+----- -----+----- -----+-----
	//      |           |           |           |
	//   nIndex1     nIndex2     nIndex3     nIndex4
	//

	for (nPos = 0; nPos < nChars; nPos += 3)
	{
		if (nPos + 1 >= nChars) cChar[nPos + 1] = '0';
		if (nPos + 2 >= nChars) cChar[nPos + 2] = '0';
		nIndex4 = ( cChar[nPos + 2] & 0x3F ) & 0x3F;
		nIndex3 = ( ((cChar[nPos + 1] & 0x0F) << 2) 
					| ((cChar[nPos + 2] & 0xC0) >> 6) ) & 0x3F;
		nIndex2 = ( ((cChar[nPos] & 3) << 4) 
					| ((cChar[nPos + 1] & 0xF0) >> 4) ) & 0x3F;
		nIndex1 = ( (cChar[nPos] & 0xFC) >> 2 ) & 0x3F;
		if (nPos + 1 >= nChars)
		{
			nIndex3 = 64;
			nIndex4 = 64;
		} // end if
		if (nPos + 2 >= nChars)
		{
			nIndex4 = 64;
		} // end if
	
        
        sprintf(st_temp, "%c%c%c%c",
					 cTable[nIndex1],
					 cTable[nIndex2],
					 cTable[nIndex3],
					 cTable[nIndex4]);
		sBase64 += st_temp;
	} // End for

    SAFE_DELETE_ARRAY(st_temp);
	return sBase64;
}




double get_time_difference_in_text(CString *cst_temp, time_t t_last_packet)
{
  	static time_t t_now;
	time(&t_now);
	static double dbl_elapsed_seconds;

	dbl_elapsed_seconds = difftime(t_now, t_last_packet);
	if (dbl_elapsed_seconds < 1) dbl_elapsed_seconds = 1; //avoid devide by zero, JIC.
	
	if (dbl_elapsed_seconds >= 60*60*24)
	cst_temp->Format(_T("%6.0f Day"), dbl_elapsed_seconds / (60*60*24));
     else    
	if (dbl_elapsed_seconds >= 60*60)
	cst_temp->Format(_T("%6.0f Hr"), dbl_elapsed_seconds / (60*60));
     else    
    if (dbl_elapsed_seconds >= 60) 
	cst_temp->Format(_T("%6.0f Min"), dbl_elapsed_seconds / 60);
	 else
	cst_temp->Format(_T("%6.0f Sec"), dbl_elapsed_seconds);

	  return dbl_elapsed_seconds;
}	


double get_time_difference_in_text_long(CString *cst_temp, time_t t_last_packet)
{
  	static time_t t_now;
	time(&t_now);
	static double dbl_elapsed_seconds;

	dbl_elapsed_seconds = difftime(t_now, t_last_packet);
	if (dbl_elapsed_seconds < 1) dbl_elapsed_seconds = 1; //avoid devide by zero, JIC.
	
	if (dbl_elapsed_seconds >= 60*60*24)
	cst_temp->Format(_T("%.0f days"), dbl_elapsed_seconds / (60*60*24));
     else    
	if (dbl_elapsed_seconds >= 60*60)
	cst_temp->Format(_T("%.0f hours"), dbl_elapsed_seconds / (60*60));
     else    
    if (dbl_elapsed_seconds >= 60) 
	cst_temp->Format(_T("%.0f minutes"), dbl_elapsed_seconds / 60);
	 else
	cst_temp->Format(_T("%.0f seconds"), dbl_elapsed_seconds);

	  return dbl_elapsed_seconds;
}	

CString int_to_cash(int i_amount)
{
	CString sAmount;
	sAmount.Format(_T("%d"), i_amount);
	if(sAmount.IsEmpty()) return "Error!";
	CString sAmount2;
	CString sComma(_T(","));
	int nDotPos = sAmount.Find('.');
	if(nDotPos == -1) sAmount += _T(".00");
	else 
	{	
		int nLenAfterDot = nDotPos + 3;	
		sAmount = sAmount.Left(nLenAfterDot);
		if(sAmount.GetLength() < nLenAfterDot) sAmount += _T("0");
	}
	
	int nAmtLen = sAmount.GetLength();
	nDotPos = sAmount.Find('.');
	int nNumThouSep = nDotPos/3;
	int nStep = nDotPos%3;
	if(nStep) sAmount2 = sAmount.Left(nStep) + sComma;
	for(int i = nStep; i < nDotPos; i+= 3)
	{
		sAmount2 += sAmount.Mid(i,3) + sComma;
	}
	
	sAmount = sAmount2.Left(sAmount2.GetLength()-1);
	//+ sAmount.Right(3);
	//uh, actually why don't remove the cents part.. 
  	//sAmount = sAmount.Left(3);

	return sAmount;
}


CString ulong_to_cash(unsigned long i_amount)
{
	CString sAmount;
	sAmount.Format(_T("%d"), i_amount);
	if(sAmount.IsEmpty()) return "Error!";
	CString sAmount2;
	CString sComma(_T(","));
	int nDotPos = sAmount.Find('.');
	if(nDotPos == -1) sAmount += _T(".00");
	else 
	{	
		int nLenAfterDot = nDotPos + 3;	
		sAmount = sAmount.Left(nLenAfterDot);
		if(sAmount.GetLength() < nLenAfterDot) sAmount += _T("0");
	}
	
	int nAmtLen = sAmount.GetLength();
	nDotPos = sAmount.Find('.');
	int nNumThouSep = nDotPos/3;
	int nStep = nDotPos%3;
	if(nStep) sAmount2 = sAmount.Left(nStep) + sComma;
	for(int i = nStep; i < nDotPos; i+= 3)
	{
		sAmount2 += sAmount.Mid(i,3) + sComma;
	}
	
	sAmount = sAmount2.Left(sAmount2.GetLength()-1);
	//+ sAmount.Right(3);
	//uh, actually why don't remove the cents part.. 
  	//sAmount = sAmount.Left(3);

	return sAmount;
}


CString float_to_cash(float i_amount)
{
	CString sAmount;
	sAmount.Format(_T("%f"), i_amount);
	if(sAmount.IsEmpty()) return "Error!";
	CString sAmount2;
	CString sComma(_T(","));
	int nDotPos = sAmount.Find('.');
	if(nDotPos == -1) sAmount += _T(".00");
	else 
	{	
		int nLenAfterDot = nDotPos + 3;	
		sAmount = sAmount.Left(nLenAfterDot);
		if(sAmount.GetLength() < nLenAfterDot) sAmount += _T("0");
	}
	
	int nAmtLen = sAmount.GetLength();
	nDotPos = sAmount.Find('.');
	int nNumThouSep = nDotPos/3;
	int nStep = nDotPos%3;
	if(nStep) sAmount2 = sAmount.Left(nStep) + sComma;
	for(int i = nStep; i < nDotPos; i+= 3)
	{
		sAmount2 += sAmount.Mid(i,3) + sComma;
	}
	
	sAmount = sAmount2.Left(sAmount2.GetLength()-1) + sAmount.Right(3);
  return sAmount;
}


//FOR unicode and normal

//set's a combo box's selection by matching it's data, not id
int set_selection_by_data(CComboBox * m_list, int i_sel)
{
	int i_count = m_list->GetCount();

	for (int i=0; i < i_count; i++)
	{
	  if (m_list->GetItemData(i) == DWORD(i_sel))
	  {
	    //found it
		  m_list->SetCurSel(i);
		  return i;
	  }

	}
  //error can't find anything with that data
	return -1;

}

int set_selection_by_data(CListBox * m_list, int i_sel)
{
	int i_count = m_list->GetCount();

	for (int i=0; i < i_count; i++)
	{
	  if (m_list->GetItemData(i) == DWORD(i_sel))
	  {
	    //found it
		  m_list->SetCurSel(i);
		  return i;
	  }

	}
  //error can't find anything with that data
	return -1;

}

 int set_selection_by_data(CListCtrl * m_list, int i_sel)
{
	int i_count = m_list->GetItemCount();

	for (int i=0; i < i_count; i++)
	{
	  if (m_list->GetItemData(i) == DWORD(i_sel))
	  {
	    //found it
		 // m_list->SetSelectionMark(i);
	      m_list->SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED |
        LVIS_FOCUSED);

          return i;
	  }

	}
  //error can't find anything with that data
	return -1;

}



bool AddText(CString cst_message, CString cst_filename)
{

   FILE *          fp = NULL;
     if (exist( (LPCTSTR) cst_filename) == false)
    {
        
        fp = _tfopen(cst_filename, _T("wb"));
        if (!fp)
        {
            LogError(_T("Can't open %s to write"),cst_filename);
            return false;
        }
        
        //let's mark it so text editors can understand that it is unicode
        CHAR *p_st_unicode = "ÿþ";

        fwrite(p_st_unicode, 2, 1, fp);
        
#ifdef _UNICODE
        fwrite( cst_message, wcslen(cst_message) * sizeof(WCHAR), 1, fp);       /* current player */
#else
        
        fwrite( uni(cst_message).us_data, _tcslen(cst_message) * sizeof(WCHAR), 1, fp);       /* current player */
#endif
        
        fclose(fp);
        return true;
    } else
    {
        fp = _tfopen(cst_filename, _T("ab"));
  #ifdef _UNICODE
      fwrite( cst_message, wcslen(cst_message)* sizeof(WCHAR), 1, fp);       /* current player */
 #else
            fwrite( uni(cst_message).us_data, _tcslen(cst_message)* sizeof(WCHAR), 1, fp);       /* current player */
  #endif

      fclose(fp);
  }



    return true;
}



CHARFORMATA CharFormatWToA(CHARFORMATW input)
{
	CHARFORMATA output;

	//this is safe because these are identical up until the string part which we'll correct in a bit
	memcpy(&output, &input, sizeof(CHARFORMATA));
	//fix the string
	strcpy(output.szFaceName, uni(input.szFaceName).to_st());

	return output; //copy the whole thing out, hey, nobody claimed this was supposed to be fast
}


CHARFORMATW CharFormatAToW(CHARFORMATA input)
{
	CHARFORMATW output;

	//this is safe because these are identical up until the string part which we'll correct in a bit
	memcpy(&output, &input, sizeof(CHARFORMATA));
	//fix the string
	wcscpy(output.szFaceName, uni(input.szFaceName).us_data);
	return output; //copy the whole thing out, hey, nobody claimed this was supposed to be fast
}

