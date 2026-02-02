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
#include "MailMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMailMessage::CMailMessage()
{
m_sFromDisplay = "";
m_sBody=_T("");
m_sHeader=_T("");
}

CMailMessage::~CMailMessage()
{

}

BOOL CMailMessage::AddRecipient( LPCTSTR szEmailAddress, LPCTSTR szFriendlyName)
{
	ASSERT( szEmailAddress != NULL );
	ASSERT( szFriendlyName != NULL );
	CRecipient to;
	to.m_sEmailAddress = szEmailAddress;
	to.m_sFriendlyName = szFriendlyName;
	m_Recipients.Add( to );
	return TRUE;
}

// sEmailAddress and sFriendlyName are OUTPUT parameters.
// If the function fails, it will return FALSE, and the OUTPUT
// parameters will not be touched.
BOOL CMailMessage::GetRecipient(CString & sEmailAddress, CString & sFriendlyName, int nIndex)
{
	CRecipient to;
	if( nIndex < 0 || nIndex > m_Recipients.GetUpperBound() )
		return FALSE;
	to = m_Recipients[ nIndex ];
	sEmailAddress = to.m_sEmailAddress;
	sFriendlyName = to.m_sFriendlyName;
	return TRUE;
}

int CMailMessage::GetNumRecipients()
{
	return m_Recipients.GetSize();
}

BOOL CMailMessage::AddMultipleRecipients(LPCTSTR szRecipients )
{
	TCHAR* buf;
	UINT pos;
	UINT start;
	CString sTemp;
	CString sEmail;
	CString sFriendly;
	UINT length;
	int nMark;
	int nMark2;

	ASSERT( szRecipients != NULL );
	
	// Add Recipients
	//
	length = _tcslen( szRecipients );
	buf = new TCHAR[ length + 1 ];	// Allocate a work area (don't touch parameter itself)
	_tcscpy( buf, szRecipients );
	for( pos = 0, start = 0; pos <= length; pos++ )
	{
		if( buf[ pos ] == ';' ||
			buf[ pos ] == 0 )
		{
			// First, pick apart the sub-strings (separated by ';')
			//  Store it in sTemp.
			//
			buf[ pos ] = 0;	// Redundant when at the end of string, but who cares.
			sTemp = &buf[ start ];

			// Now divide the substring into friendly names and e-mail addresses.
			//
			nMark = sTemp.Find( '<' );
			if( nMark >= 0 )
			{
				sFriendly = sTemp.Left( nMark );
				nMark2 = sTemp.Find( '>' );
				if( nMark2 < nMark )
				{
					delete[] buf;
					return FALSE;
				}
				// End of mark at closing bracket or end of string
				nMark2 > -1 ? nMark2 = nMark2 : nMark2 = sTemp.GetLength() - 1;
				sEmail = sTemp.Mid( nMark + 1, nMark2 - (nMark + 1) );
			}
			else
			{
				sEmail = sTemp;
				sFriendly = "";
			}
			AddRecipient( sEmail, sFriendly );
			start = pos + 1;
		}
	}
	delete[] buf;
	return TRUE;
}


BOOL CMailMessage::EncodeHeader()
{
	CString sTo;
	CString sDate;

	if( GetNumRecipients() <= 0 )
		return FALSE;

	m_sHeader = "";	// Clear it

	// Get the recipients into a single string
	sTo = "";
	CString sEmail = "";
	CString sFriendly = "";
	for( int i = 0; i < GetNumRecipients(); i++ )
	{
		GetRecipient( sEmail, sFriendly, i );
		sTo += ( i > 0 ? "," : "" );
		sTo += sFriendly;
		sTo += "<";
		sTo += sEmail;
		sTo += ">";
	}
	m_tDateTime = m_tDateTime.GetCurrentTime();
	// Format: Mon, 01 Jun 98 01:10:30 GMT
	//sDate = m_tDateTime.Format( "%a, %d %b %y %H:%M:%S %Z" );

	//fixed to use global time
	sDate = m_tDateTime.FormatGmt( "%a, %d %b %y %H:%M:%S %Z" ); 
	
	
	m_sHeader.Format( _T("Date: %s\r\n")\
					_T("From: %s\r\n")\
					_T("To: %s\r\n")\
					_T("Subject: %s\r\n"),
					// Include other extension lines if desired
					(LPCTSTR)sDate,
					(LPCTSTR)m_sFromDisplay,
					(LPCTSTR)sTo,
					(LPCTSTR)m_sSubject);
	return TRUE;
}


BOOL CMailMessage::DecodeHeader()
{
	int startpos, endpos;
	CString sSearchFor;

    CString cst_temp_header = m_sHeader;
    cst_temp_header.MakeUpper();

	//We can assume that there's a CR/LF before each of the tags, as the servers insert
	//Received: lines on top of the mail while transporting the mail
	sSearchFor="\nFROM: ";
	startpos=cst_temp_header.Find(sSearchFor);
	if (startpos<0)
    {
      m_sFrom = "Unknown";
    }   else
    {
    
	endpos=m_sHeader.Mid(startpos+sSearchFor.GetLength()).Find(_T("\n"));
	m_sFrom=m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos);
    
    //remove any /r's
    m_sFrom.Replace(_T("\r"), _T(""));
    }

	sSearchFor="\nTO: ";
	startpos=cst_temp_header.Find(sSearchFor);
	if (startpos >= 0)
    {
	endpos=m_sHeader.Mid(startpos+sSearchFor.GetLength()).Find(_T("\n"));
	AddMultipleRecipients(m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos));
    this->m_sFrom.Replace(_T("\r"), _T(""));
    } else
    {
        //unable to find to address

    }
	sSearchFor="\nDATE: ";
	startpos=cst_temp_header.Find(sSearchFor);
	//if (startpos<0) return FALSE;

    if (startpos != -1)
    {
    
    endpos = m_sHeader.Mid(startpos+sSearchFor.GetLength()).Find(_T("\n"));
	//DATE=m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos));
	//This is incorrect ! We have to parse the Date: line !!!
	//Anyone likes to write a parser for the different formats a date string may have ?

    if (endpos != -1)
    {
    

    //Added by Seth, I know this is slow.. this whole section should be written.
    m_sDate = m_sHeader.Mid(startpos+sSearchFor.GetLength(), endpos);
     m_sDate.Replace(_T("\r"), _T(""));

     if (m_sDate.GetLength() < 3)
     {
         //no date found, let's force out own
         m_sDate = _T("Error, no date found");
     }
    } else
    {

      m_sDate = _T("Error, no date found");

    }
    
    
    } else
    {

    m_sDate = _T("Error, no date found");
    }
     
     
     m_tDateTime = m_tDateTime.GetCurrentTime();

	sSearchFor="\r\nSUBJECT: ";
	startpos=cst_temp_header.Find(sSearchFor);
	if (startpos<0)
    {
     m_sSubject = "no subject";
    } else
    {
    
	endpos=m_sHeader.Mid(startpos+sSearchFor.GetLength()).Find(_T("\r\n"));
	m_sSubject=m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos);
    }

	//ATTENTION: Cc parsing won't work, if Cc is split up in multiple lines
	// Cc: recipient1 <rec1@ab.cd>,
	//	   recipient2 <rec2@ab.cd>,
	//	   recipient3 <rec3@ab.cd>
	// won't work !!!
	sSearchFor="\r\nCC: ";
	startpos=cst_temp_header.Find(sSearchFor);
	if (startpos>=0) //no error if there's no Cc
	{
		endpos=m_sHeader.Mid(startpos+sSearchFor.GetLength()).Find(_T("\r\n"));
		AddMultipleRecipients(m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos));
	}

	return TRUE;
}


void CMailMessage::EncodeBody()
{
	CString sCooked = "";
	LPTSTR szBad = _T("\r\n.\r\n");
	LPTSTR szGood = _T("\r\n..\r\n");
	int nPos;
	int nBadLength = _tcslen( szBad );
	if( m_sBody.Left( 3 ) == ".\r\n" )
		m_sBody = "." + m_sBody;
	//
	// This is a little inefficient because it beings a search
	// at the beginning of the string each time. This was
	// the only thing I could think of that handled ALL variations.
	// In particular, the sequence "\r\n.\r\n.\r\n" is troublesome. 
	// (Even CStringEx's FindReplace wouldn't handle that situation
	// with the global flag set.)
	//
	while( (nPos = m_sBody.Find( szBad )) > -1 )
	{
		sCooked = m_sBody.Mid( 0, nPos );
		sCooked += szGood;
		m_sBody = sCooked + m_sBody.Right( m_sBody.GetLength() - (nPos + nBadLength) );
	}
}


void CMailMessage::DecodeBody()
{
	CString sCooked = "";
	LPTSTR szBad = _T("\r\n..\r\n");
	LPTSTR szGood = _T("\r\n.\r\n");
	int nPos;
	int nBadLength = _tcslen( szBad );
	if( m_sBody.Left( 4 ) == "..\r\n" )
		m_sBody = m_sBody.Mid(1);
	//
	// This is a little inefficient because it beings a search
	// at the beginning of the string each time. This was
	// the only thing I could think of that handled ALL variations.
	// In particular, the sequence "\r\n.\r\n.\r\n" is troublesome. 
	// (Even CStringEx's FindReplace wouldn't handle that situation
	// with the global flag set.)
	//
	while( (nPos = m_sBody.Find( szBad )) > -1 )
	{
		sCooked = m_sBody.Mid( 0, nPos );
		sCooked += szGood;
		m_sBody = sCooked + m_sBody.Right( m_sBody.GetLength() - (nPos + nBadLength) );
	}
}