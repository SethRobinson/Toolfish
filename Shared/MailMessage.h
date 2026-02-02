/* Copyright (C) Seth A. Robinson, 2003. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Seth A. Robinson, 2003"
 */



// MailMessage.h: interface for the CMailMessage class.
// Copyright (c) 1998, Wes Clyburn
//
// Modified to have Header and Body handling in this class rather than in any
// class that uses instances of CMailMessage.
// Copyright (c) 1998 Michael Krebs
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_)
#define AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>

class CMailMessage  
{
public:
	CMailMessage();
	virtual ~CMailMessage();

	int GetNumRecipients();
	BOOL GetRecipient( CString& sEmailAddress, CString& sFriendlyName, int nIndex = 0 );
	BOOL AddRecipient( LPCTSTR szEmailAddress, LPCTSTR szFriendlyName = _T("") );
	BOOL AddMultipleRecipients( LPCTSTR szRecipients = NULL );

	BOOL EncodeHeader();// Create the SMTP message header as per RFC822
	BOOL DecodeHeader();// Read fields from Header - NOT COMPLETED

	void EncodeBody(); // Exchange .CR/LF by ..CR/LF
	void DecodeBody(); // There's no Base64/Mime/UU en- or decoding done here !

	CString m_sFrom;
	CString m_sFromDisplay;
	CString m_sSubject;
	CString m_sHeader;
	CTime m_tDateTime;	
	CString m_sBody;
    CString m_sDate; //Added by Seth

private:
	class CRecipient
	{
		public:
			CString m_sEmailAddress;
			CString m_sFriendlyName;
	};
	CArray <CRecipient, CRecipient&> m_Recipients;
};

#endif // !defined(AFX_MAILMESSAGE_H__55DE48CC_BEA4_11D1_870E_444553540000__INCLUDED_)
