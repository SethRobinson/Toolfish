#include "winsock_util.h"
#include <iterator>
/** web.h
* 1.Declares a class to encode strings converting a String 
* into a MIME format called "x-www-form-urlencoded" format. 
*	To convert a String, each character is examined in turn: 
*		1) The ASCII characters 'a' through 'z', 'A' through 'Z', and '0' through '9' remain the same. 
* 	2) The space character ' ' is converted into a plus sign '+'. 
*		3) All other characters are converted into the 3-character string "%xy", where xy is the two-digit hexadecimal representation of the lower 8-bits of the character. 
* 2.Declares a class to decode such strings
* 3. Declares the WebForm class to wrap win32 HTTP calls
* Author: Vijay Mathew Pandyalakal
* Date: 18/10/03

Stolen from CodeProject
**/

 


void DecToHexString ( ULONG value, byte * pOut, WORD charArrayMaxSize) 
{ 
    static byte  digit; 
    static int  i; 

    for (i = charArrayMaxSize - 1; i >= 0; i--) 
    { 
        digit = byte((value & 0x0f) + 0x30);
        if (digit > 0x39) digit += 0x07; 
        pOut[i] = digit; 
        value >>= 4; 
    } 
} 


void URLEncoder::encodeData(const byte *pData, int len, string &finalReturn)
{
    char tmp[6];
    tmp[0] = '%';
    tmp[3] = 0;
    assert(len != 0 && len != -1 && "You need to send the length too.");
    string ret = "";

    vector<char> buff;
    buff.reserve(len*2);
       
    for(int i=0;i<len;i++) 
    {
        if(isOrdinaryChar((char)pData[i])) 
        {
            buff.push_back((char)pData[i]);
        }else if(pData[i] == ' ') 
        {
            buff.push_back('+');
        }else 
        {
             DecToHexString(pData[i], (byte*)&tmp[1], 2); //SETH, this should be faster than sprintf.  start after the % part
            //sprintf(tmp,"%%%02x",pData[i]); //SETH added 02 for leading zeros, serious bug fix.  Sent fix to author
            buff.push_back(tmp[0]);
            buff.push_back(tmp[1]);
            buff.push_back(tmp[2]);
        }
    }
    
    ret.reserve(ret.size()+buff.size()); //minimize mem allocations
    back_insert_iterator<string> dest (finalReturn);
    copy (buff.begin(), buff.end(), dest);
}


bool URLEncoder::isOrdinaryChar(char c) 
{
    if (
        (c >= 48 && c <= 57) ||
        (c >= 65 && c <= 90) ||
        (c >= 97 && c <= 122)) return true;
		return false;
}

string URLDecoder::decode(string str) 
{
	int len = str.length();
	char* buff = new char[len + 1];
	strcpy(buff,str.c_str());
	string ret = "";
	for(int i=0;i<len;i++) 
	{
		if(buff[i] == '+')
		{
			ret = ret + " ";
		}else if(buff[i] == '%')
		{
			char tmp[4];
			char hex[4];			
			hex[0] = buff[++i];
			hex[1] = buff[++i];
			hex[2] = '\0';		
			//int hex_i = atoi(hex);
			sprintf(tmp,"%c",convertToDec(hex));
			ret = ret + tmp;
		}else {
			ret = ret + buff[i];
		}
	}
	delete[] buff;
	return ret;
}

vector<byte> URLDecoder::decodeData(const string str) 
{
    int len = str.length();
    vector<byte> outBuff;
    outBuff.reserve(len);
    char hex[4];			
    hex[2] = '\0';		

    for(int i=0;i<len;i++) 
    {
        if(str[i] == '+')
        {
            outBuff.push_back(' ');
        }else if(str[i] == '%')
        {
            hex[0] = str[++i];
            hex[1] = str[++i];
            //int hex_i = atoi(hex);
            outBuff.push_back(byte(convertToDec(hex)));
        }  else 
        {
            outBuff.push_back(str[i]);
        }
    }

    return outBuff;
}
int URLDecoder::convertToDec(const char* hex)
{
	char buff[12];
	sprintf(buff,"%s",hex);
	int ret = 0;
	int len = strlen(buff);
	for(int i=0;i<len;i++) 
	{
		char tmp[4];
		tmp[0] = buff[i];
		tmp[1] = '\0';
		getAsDec(tmp);
		int tmp_i = atoi(tmp);
		int rs = 1;
		for(int j=i;j<(len-1);j++)
		{
			rs *= 16;
		}
		ret += (rs * tmp_i);
	}
	return ret;
}

void URLDecoder::getAsDec(char* hex) {
	char tmp = tolower(hex[0]);
	if(tmp == 'a') {
		strcpy(hex,"10");
	}else if(tmp == 'b') {
		strcpy(hex,"11");
	}else if(tmp == 'c') {
		strcpy(hex,"12");
	}else if(tmp == 'd') {
		strcpy(hex,"13");
	}else if(tmp == 'e') {
		strcpy(hex,"14");
	}else if(tmp == 'f') {
		strcpy(hex,"15");
	}else if(tmp == 'g') {
		strcpy(hex,"16");
	}
}
bool GetLocalAddress(char * p_st_out_final)
{
    struct in_addr *pinAddr;
    LPHOSTENT	lpHostEnt;
	int			nRet;


	// Get our local name
	//
    char st_buff[MAX_PATH];
    
    nRet = gethostname((char*)&st_buff, MAX_PATH);
	if (nRet == SOCKET_ERROR)
	{
	    log_error("Unable to figure out our own IP address.");
        strcpy(st_buff, "(no ip?)");
		return false;
	}

	//
	// "Lookup" the local name
	//
	lpHostEnt = gethostbyname((char*)&st_buff);
    if (lpHostEnt == NULL)
	{
	       log_error("Unable to figure out our own IP address.");
      		strcpy(st_buff, "(no ip?)");
		    return false;
	}

	//
    // Format first address in the list
	//
	
    //first count how many addresses we have
    int i_nics = 0;
	int i;
    for (int i=0; lpHostEnt->h_addr_list[i]; i++ )
    {
      i_nics++;
    }

    p_st_out_final[0] = 0; //clear this
    //ok, now that we know how many let's do this
    int i_chosen_nic = -1; //-1 will signal an error
   
	for (i=0; lpHostEnt->h_addr_list[i]; i++ )
    {
        
        if (byte(lpHostEnt->h_addr_list[i][0]) == 192) if (byte(lpHostEnt->h_addr_list[i][1]) == 168)
       {
           //this is some kind of local address, ignore it if we have a choice
           if (i_nics > 1) continue;
       }
        if (byte(lpHostEnt->h_addr_list[i][0]) == 169) if (byte(lpHostEnt->h_addr_list[i][1]) == 254)
       {
           //this is some kind of local address, ignore it if we have a choice
           if (i_nics > 1) continue;
       }
        if (byte(lpHostEnt->h_addr_list[i][0]) == 6) if (byte(lpHostEnt->h_addr_list[i][1]) == 0)
        {
            //this is some kind of local address, ignore it if we have a choice
            if (i_nics > 1) continue;
        }
        
        i_chosen_nic = i;
  
    }
    if (i_chosen_nic == -1)
    {
       // LogMsg(_T("Unable to find an IP address."));
         strcpy(st_buff, "(no ip?)");
         return false;
    }


  //  log_msg("Found %d IP's, choosing #%d.", i_nics, i_chosen_nic);
    pinAddr = ((LPIN_ADDR)lpHostEnt->h_addr_list[i_chosen_nic]);
  	strcpy(p_st_out_final, inet_ntoa(*pinAddr));
    return true;
}    





