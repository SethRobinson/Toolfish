
//#include "all.h"
#include "mfc_all.h"
#include "POP3.h"  
#include "uni.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TCHAR* CPOP3::error_table[] =
{
	_T("Server didn't connect"),
	_T("Bad user name"),							// IDENTIFICATION
	_T("Invalid username/password combination"),	// AUTHENTIFICATION
	_T("STATus couldn't be retrieved"),				// STATUS
	_T("RETRieve failed"),							// RETRIEVE
	_T("Could not DELEte message"),					// DELE
	_T("Error while QUITting")						// QUIT
};
//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CPOP3::CPOP3( LPCTSTR szPOP3ServerName, UINT nPort, LPCTSTR sUsername, LPCTSTR sPassword)
{
	ASSERT( szPOP3ServerName != NULL );
	AfxSocketInit();
	m_sPOP3ServerHostName = szPOP3ServerName;
	m_nPort = nPort;
	m_sUsername = sUsername;
	m_sPassword = sPassword;
	m_bConnected = FALSE;
	m_sError = _T( "OK" );

}

CPOP3::~CPOP3()
{
	if( m_bConnected )
		Disconnect();
}


BOOL CPOP3::Connect()
{
	CString sUser;
	CString sPass;

	if( m_bConnected )
		return TRUE;

	if( !m_wsPOP3Server.Create() )
	{
		m_sError = _T( "Unable to create the socket." );
		return FALSE;
	}


	
	if( !m_wsPOP3Server.Connect( GetServerHostName(), GetPort() ) )
	{
		m_sError = _T( "Unable to connect to server" );
		m_wsPOP3Server.Close();
		return FALSE;
	}
	if( !get_response( CONNECTION ) )
	{
		m_wsPOP3Server.Close();
		return FALSE;
	}
	
    
    sUser.Format( _T("USER %s\r\n"), GetUsername());
#ifdef _UNICODE
    m_wsPOP3Server.Send( (LPCTSTR)uni(sUser).to_st(), sUser.GetLength() );

#else
    
    m_wsPOP3Server.Send( (LPCTSTR)sUser, sUser.GetLength() );
#endif
    if( !get_response( IDENTIFICATION ) )
	{
		m_wsPOP3Server.Close();
		return FALSE;
	}

    sPass.Format( _T("PASS %s\r\n"), GetPassword());
	
  #ifdef _UNICODE
   m_wsPOP3Server.Send( (LPCTSTR)uni(sPass).to_st(), sPass.GetLength() );


#else
  
    m_wsPOP3Server.Send( (LPCTSTR)sPass, sPass.GetLength() );
#endif
	if( !get_response( AUTHENTIFICATION ) )
	{
		m_wsPOP3Server.Close();
		return FALSE;
	}

	m_bConnected = TRUE;
	return TRUE;
}


BOOL CPOP3::get_response( UINT executed_action )
{
    int nChars = m_wsPOP3Server.Receive( response_buf, RESPONSE_BUFFER_SIZE );
	if( nChars == SOCKET_ERROR )
	{
		m_sError = _T( "Socket Error" );
		return FALSE;
	}

	m_sResponse = response_buf;
	m_sResponse = m_sResponse.Left(nChars);

	if (m_sResponse.Left(4)=="-ERR")
	{
		m_sError=error_table[executed_action];
		return FALSE; 
	}
	return TRUE;
}


UINT CPOP3::GetPort()
{
	return m_nPort;
}


CString CPOP3::GetUsername()
{
	return m_sUsername;
}


CString CPOP3::GetPassword()
{
	return m_sPassword;
}


CString CPOP3::GetLastError()
{
	return m_sError;
}


CString CPOP3::GetServerHostName()
{
	return m_sPOP3ServerHostName;
}


void CPOP3::SetServerProperties( LPCTSTR sServerHostName, UINT nPort)
{
	ASSERT( sServerHostName != NULL );
	// Needs to be safe in non-debug too
	if( sServerHostName == NULL )
		return;
	m_sPOP3ServerHostName = sServerHostName;
	m_nPort = nPort;
}


void CPOP3::SetUserProperties( LPCTSTR sUsername, LPCTSTR sPassword )
{
	ASSERT( sUsername != NULL );
	ASSERT( sPassword != NULL );
	
	if( sUsername == NULL )
		return;
	if( sPassword == NULL )
		return;
	
	m_sUsername = sUsername;
	m_sPassword = sPassword;
}


BOOL CPOP3::Disconnect()
{
	BOOL ret;
	if( !m_bConnected )
		return TRUE;
	// Disconnect gracefully from the server and close the socket
	CString sQuit = _T( "QUIT\r\n" );
#ifdef _UNICODE
    m_wsPOP3Server.Send( (LPCTSTR)uni(sQuit).to_st(), sQuit.GetLength() );

#else
    m_wsPOP3Server.Send( (LPCTSTR)sQuit, sQuit.GetLength() );

#endif

	// No need to check return value here.
	// If it fails, the message is available with GetLastError
	ret = get_response( QUIT );
	m_wsPOP3Server.Close();

	m_bConnected = FALSE;
	return ret;
}


int CPOP3::GetNumMessages()
{
	CString sStat = _T( "STAT\r\n" );
#ifdef _UNICODE
    m_wsPOP3Server.Send( (LPCTSTR)uni(sStat).to_st(), sStat.GetLength() );
#else
     m_wsPOP3Server.Send( (LPCTSTR)sStat, sStat.GetLength() );

#endif

	if( !get_response( STATUS ) ) return -1;

	int pos=m_sResponse.FindOneOf(_T("0123456789"));
	if (pos<0) return -1;
	return _ttoi(m_sResponse.Mid(pos));

}


BOOL CPOP3::GetMessage( UINT nMsg, CMailMessage* msg)
{
	CString sMsg;
	CString sRetr;
	sRetr.Format(_T("RETR %d\r\n"),nMsg);
#ifdef _UNICODE
    m_wsPOP3Server.Send( (LPCTSTR)uni(sRetr).to_st(), sRetr.GetLength() );

#else
    m_wsPOP3Server.Send( (LPCTSTR)sRetr, sRetr.GetLength() );

#endif

	if( !get_response( RETRIEVE ) ) return FALSE;

	sMsg=m_sResponse;
	while ( sMsg.Find(_T("\r\n.\r\n"))<0 ) 
	{
		// nChars = number of bytes read
		int nChars = m_wsPOP3Server.Receive( response_buf, RESPONSE_BUFFER_SIZE );
		if ( nChars == SOCKET_ERROR ) return FALSE;
		m_sResponse=response_buf;
		sMsg+=m_sResponse.Left( nChars ); //only the first nChars bytes of response_buf are valid !
	}

	sMsg=sMsg.Mid(sMsg.Find(_T("\r\n"))+2); //first line of output is +OK
	sMsg=sMsg.Left(sMsg.GetLength()-3); //last line is always .\r\n

	int br=sMsg.Find(_T("\r\n\r\n")); //breakpoint between header and body
	msg->m_sHeader=sMsg.Left(br);
	msg->m_sBody=sMsg.Mid(br+4);
	msg->DecodeHeader();
	msg->DecodeBody();
	return TRUE;
}

BOOL CPOP3::DeleteMessage( UINT nMsg )
{
	CString sDele = _T( "STAT\r\n" );

    sDele.Format(_T("DELE %d\r\n"),nMsg);
#ifdef _UNICODE
    m_wsPOP3Server.Send( (LPCTSTR)uni(sDele).to_st(), sDele.GetLength() );

#else
    m_wsPOP3Server.Send( (LPCTSTR)sDele, sDele.GetLength() );

#endif

	return get_response( DELE );
}

//same as above but will only grab the first part of the message

BOOL CPOP3::GetMessageTop( UINT nMsg, int i_body_lines, CMailMessage* msg)
{
	CString sMsg;
	CString sRetr;
	sRetr.Format(_T("TOP %d %d\r\n"),nMsg, i_body_lines);
#ifdef _UNICODE
    m_wsPOP3Server.Send( (LPCTSTR)uni(sRetr).to_st(), sRetr.GetLength() );

#else
    m_wsPOP3Server.Send( (LPCTSTR)sRetr, sRetr.GetLength() );

#endif

	if( !get_response( RETRIEVE ) ) return FALSE;

	sMsg=m_sResponse;
	while ( sMsg.Find(_T("\r\n.\r\n"))<0 ) 
	{
		// nChars = number of bytes read
		int nChars = m_wsPOP3Server.Receive( response_buf, RESPONSE_BUFFER_SIZE );
		if ( nChars == SOCKET_ERROR ) return FALSE;
		m_sResponse=response_buf;
		sMsg+=m_sResponse.Left( nChars ); //only the first nChars bytes of response_buf are valid !
	}

	sMsg=sMsg.Mid(sMsg.Find(_T("\r\n"))+2); //first line of output is +OK
	sMsg=sMsg.Left(sMsg.GetLength()-3); //last line is always .\r\n

	int br=sMsg.Find(_T("\r\n\r\n")); //breakpoint between header and body
	
	if (br != -1)
	{
	msg->m_sHeader=sMsg.Left(br);
	msg->m_sBody=sMsg.Mid(br+4);
	} else
	{
		//no message content?!
	}
	
	//apend a final dohicky on the header, some times it's not there already
    msg->m_sHeader += "\r\n";
    
    
    msg->DecodeHeader();
	msg->DecodeBody();
	return TRUE;
}



