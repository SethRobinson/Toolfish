#pragma once
#include "..\all.h"
//#include "winsock2.h"

#include <string>
#include <vector>
using namespace std;

class URLEncoder 
{
public:
    void encodeData(const byte *pData, int len, string &finalReturn);

private:
	static bool isOrdinaryChar(char c);
};

class URLDecoder 
{
public:
	static string decode(string str);
    vector<byte> decodeData(const string str);

private:
	static int convertToDec(const char* hex);
	static void getAsDec(char* hex);
};

bool GetLocalAddress(char * p_st_out_final);

