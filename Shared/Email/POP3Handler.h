// POP3Handler.h: Schnittstelle für die Klasse CPOP3Handler.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POP3HANDLER_H__EB3CAB35_0595_4616_91D5_EBC806F5F2ED__INCLUDED_)
#define AFX_POP3HANDLER_H__EB3CAB35_0595_4616_91D5_EBC806F5F2ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxsock.h>

enum DataTypes
	{
	MAIL_AS_STRING,
	TOP_AS_STRING,
	LIST_AS_STRING
	};

class CPOP3Handler  
	{
	public:
		CPOP3Handler();
		virtual ~CPOP3Handler();

	protected:
		enum ValidationType
			{
			CONNECTION_CHECK,
			USER_CHECK,
			PASSWORD_CHECK,
			QUIT_CHECK,
			DELETE_CHECK,
			RSET_CHECK,
			STAT_CHECK,
			NOOP_CHECK,
			LIST_CHECK,
			RETR_CHECK,
			TOP_CHECK
			};

		// Temporary Files for storing Data
		char	   m_ListPath [MAX_PATH];
		char	   m_OutputFilePath [MAX_PATH];

		CStdioFile m_ListFile;
		CStdioFile m_OutputFile;

	protected:
		CString		m_ErrorMessage;
		CSocket		m_POP3Server;

		int	m_Number_NewMails;
		int m_TotalMailSize;

	public:
		bool Connect(LPCTSTR p_User, LPCTSTR p_Password, LPCTSTR p_Host, int p_Port = 110);
		bool Disconnect();

		bool CheckResponse (int p_ResponseType);

		bool Statistics ();
		bool Delete		(int p_MsgNumber);
		bool Noop		();
		bool Reset		();
		bool Retrieve	(int p_MsgNumber);
		bool Retrieve	(int p_MsgNumber, LPCTSTR p_pTargetFileName);
		bool List		();
		bool GetTop		(int p_MsgNumber, int p_Length);
		bool SetOutputFile	(LPCTSTR p_OutputFileName);

		int		GetNumberNewMails ();
		int		GetTotalMailSize  ();

		CString Get				(int p_DataType, int p_MessageNumber = 1);
		CString GetErrorMessage ();
	};

#endif // !defined(AFX_POP3HANDLER_H__EB3CAB35_0595_4616_91D5_EBC806F5F2ED__INCLUDED_)
