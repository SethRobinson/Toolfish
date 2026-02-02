// Registry.cpp: implementation of the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "mfc_all.h"
#include "Registry.h"
#include "uni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistry::CRegistry(HKEY hKey)
{
	ASSERT(hKey);

	m_hKey = hKey;
}

CRegistry::~CRegistry()
{
	Close();
}

BOOL CRegistry::Open(HKEY hKey, LPCTSTR lpszPath)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	m_sPath = lpszPath;

	LONG lResult = RegOpenKeyEx(hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::Open(LPCTSTR lpszPath)
{
	ASSERT(lpszPath);

	m_sPath = lpszPath;

	LONG lResult = RegOpenKeyEx(m_hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

void CRegistry::Close()
{
	if (m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

BOOL CRegistry::VerifyKey(HKEY hKey, LPCTSTR lpszPath)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	LONG lResult = RegOpenKeyEx(hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);

	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::VerifyKey(LPCTSTR lpszPath)
{
	ASSERT(lpszPath);

	LONG lResult = RegOpenKeyEx(m_hKey, lpszPath, NULL, KEY_ALL_ACCESS, &m_hKey);
	
	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::VerifyValue(LPCTSTR lpszValue)
{
	ASSERT(lpszValue);

	LONG lResult = RegQueryValueEx(m_hKey, lpszValue, NULL, NULL, NULL, NULL);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::CreateKey(HKEY hKey, LPCTSTR lpszPath, DWORD dwOptions)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	DWORD dwDisp = -1;

	LONG lResult = RegCreateKeyEx(hKey, lpszPath, NULL, NULL, dwOptions, KEY_ALL_ACCESS, NULL, &m_hKey, &dwDisp);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::DeleteKey(HKEY hKey, LPCTSTR lpszPath)
{
	ASSERT(hKey);
	ASSERT(lpszPath);

	LONG lResult = RegDeleteKey(hKey, lpszPath);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::DeleteValue(LPCTSTR lpszValue)
{
	ASSERT(lpszValue);

	LONG lResult = RegDeleteValue(m_hKey, lpszValue);

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::WriteString(LPCTSTR lpszKey, LPCTSTR lpszData)
{
	ASSERT(lpszKey);
	ASSERT(lpszData);

#ifdef _UNICODE
    LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_SZ, (const BYTE*) lpszData, (wcslen(lpszData) + 1)*2);

#else
    LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_SZ, (const BYTE*) lpszData, strlen(lpszData) + 1);
#endif
	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::ReadString(LPCTSTR lpszKey, CString &sData)
{
	ASSERT(lpszKey);

	DWORD dwType = -1;
	DWORD dwSize = -1;

#ifdef _UNICODE
    LONG lResult = RegQueryValueEx(m_hKey,  lpszKey, NULL, &dwType, NULL, &dwSize);

#else
    LONG lResult = RegQueryValueEx(m_hKey, (LPSTR) lpszKey, NULL, &dwType, NULL, &dwSize);

#endif

	if(lResult == ERROR_SUCCESS && dwType == REG_SZ)
	{
		UCHAR* pszData = new UCHAR[dwSize];
		
#ifdef _UNICODE
        lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, pszData, &dwSize);

#else
        lResult = RegQueryValueEx(m_hKey, (LPSTR) lpszKey, NULL, &dwType, pszData, &dwSize);

#endif

        if (lResult == ERROR_SUCCESS && dwType == REG_SZ)
		{
			sData.Format(_T("%s"),pszData); //Seth's fix, so it works with unicode keys right
			delete pszData;
			return TRUE;
		}
		delete pszData;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CRegistry::WriteDWORD(LPCTSTR lpszKey, const DWORD &dwData)
{
	ASSERT(lpszKey);
	
	LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_DWORD, (const BYTE*) &dwData, sizeof(DWORD));

	if(lResult == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::ReadDWORD(LPCTSTR lpszKey, DWORD &dwData)
{
	ASSERT(lpszKey);

	DWORD dwType = -1;
	DWORD dwSize = sizeof(DWORD);

#ifdef _UNICODE
    LONG lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) &dwData, &dwSize);

#else
    LONG lResult = RegQueryValueEx(m_hKey, (LPSTR) lpszKey, NULL, &dwType, (BYTE*) &dwData, &dwSize);

#endif

	if(lResult == ERROR_SUCCESS && dwType == REG_DWORD)
		return TRUE;

	return FALSE;
}

BOOL CRegistry::Write(LPCTSTR lpszKey, CWnd *pWnd)
{
	ASSERT(lpszKey);
	ASSERT(pWnd);

	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	pWnd->GetWindowPlacement(&wp);

	CMemFile file;
	CArchive ar(&file, CArchive::store);

	ar << wp.flags
		 << wp.showCmd
		 << wp.ptMinPosition
		 << wp.ptMaxPosition
		 << wp.rcNormalPosition;
	ar.Close();

	DWORD dwSize = static_cast<DWORD>(file.GetLength());
	BYTE* pByte = file.Detach();

	LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_BINARY, pByte, dwSize);

	if (pByte)
		free(pByte);

	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpszKey, CWnd* pWnd)
{
	ASSERT(lpszKey);
	ASSERT(pWnd);

	DWORD dwType;
	DWORD dwSize = sizeof(WINDOWPLACEMENT);
	UCHAR* pszData = new UCHAR[dwSize];

	WINDOWPLACEMENT wp;
	wp.length = dwSize;

	LONG lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) pszData, &dwSize);

	if(lResult == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		CMemFile file(pszData, dwSize);
		CArchive ar(&file, CArchive::load);

		ar >> wp.flags
			 >> wp.showCmd
			 >> wp.ptMinPosition
			 >> wp.ptMaxPosition
			 >> wp.rcNormalPosition;
		ar.Close();

		file.Close();
		
		pWnd->SetWindowPlacement(&wp);

		delete pszData;
		return TRUE;
	}

	delete pszData;
	return FALSE;
}

BOOL CRegistry::Write(LPCTSTR lpszKey, CObject& object)
{
	ASSERT(lpszKey);
	
	CMemFile file;
	CArchive ar(&file, CArchive::store);
	
	object.Serialize(ar);
	ar.Close();
	
	DWORD dwSize = static_cast<DWORD>(file.GetLength());
	#ifdef _DEBUG
		if (dwSize > 2048)
			TRACE1("CRegistry::Write - Object size = %d (> 2048) better use a file !\n", dwSize);
	#endif
	BYTE* pByte = file.Detach();
	
	LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_BINARY, pByte, dwSize);
	
	if (pByte)
		free(pByte);
	
	if(lResult == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CRegistry::Read(LPCTSTR lpszKey, CObject& object)
{
	ASSERT(lpszKey);
	
	DWORD dwType = -1;
	DWORD dwSize = -1;
	UCHAR *pszData = NULL;
	
	LONG lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) pszData, &dwSize);
	
	if (lResult == ERROR_SUCCESS && dwType == REG_BINARY)
		pszData = new UCHAR[dwSize];

	lResult = RegQueryValueEx(m_hKey, lpszKey, NULL, &dwType, (BYTE*) pszData, &dwSize);
	if (lResult == ERROR_SUCCESS && dwType == REG_BINARY)
	{
		CMemFile file(pszData, dwSize);
		CArchive ar(&file, CArchive::load);
		
		object.Serialize(ar);
		ar.Close();
			
		file.Close();
			
		delete pszData;
		return TRUE;
	}

	delete pszData;
	return FALSE;
}
