// Registry.h: interface for the CRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRY_H__08F26279_A63D_11D3_B73D_0050DA34A2BA__INCLUDED_)
#define AFX_REGISTRY_H__08F26279_A63D_11D3_B73D_0050DA34A2BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegistry : public CObject  
{
public:
	CRegistry(HKEY hKey = HKEY_CURRENT_USER);
	virtual ~CRegistry();

	BOOL Open(HKEY hKey, LPCTSTR lpszPath);
	BOOL Open(LPCTSTR lpszPath);
	void Close();

	BOOL VerifyKey(LPCTSTR lpszPath);
	BOOL VerifyKey(HKEY hKey, LPCTSTR lpszPath);
	BOOL VerifyValue(LPCTSTR lpszValue);

	BOOL CreateKey(HKEY hKey, LPCTSTR lpszPath, DWORD dwOptions = REG_OPTION_NON_VOLATILE);
	BOOL DeleteKey(HKEY hKey, LPCTSTR lpszPath);
	BOOL DeleteValue(LPCTSTR lpszValue);

	BOOL WriteString(LPCTSTR lpszKey, LPCTSTR lpszData);
	BOOL WriteDWORD(LPCTSTR lpszKey, const DWORD& dwData);
	BOOL Write(LPCTSTR lpszKey, CWnd* pWnd);
	BOOL Write(LPCTSTR lpszKey, CObject& object);

	BOOL ReadString(LPCTSTR lpszKey, CString& sData);
	BOOL ReadDWORD(LPCTSTR lpszKey, DWORD& dwData);
	BOOL Read(LPCTSTR lpszKey, CWnd *pWnd);
	BOOL Read(LPCTSTR lpszKey, CObject& object);

	// Templates...
	template<class T> BOOL WriteType(LPCTSTR lpszKey, T& type)
	{
		ASSERT(lpszKey);
		
		CMemFile file;
		CArchive ar(&file, CArchive::store);
		
		ar << type;
		ar.Close();
		
		DWORD dwSize = file.GetLength();
		#ifdef _DEBUG
			if (dwSize > 2048)
				TRACE1("CRegistry::WriteType - Object size = %d (> 2048) better use a file !\n", dwSize);
		#endif
		BYTE* pByte = file.Detach();
		
		LONG lResult = RegSetValueEx(m_hKey, lpszKey, NULL, REG_BINARY, pByte, dwSize);
		
		if (pByte)
			free(pByte);
		
		if(lResult == ERROR_SUCCESS)
			return TRUE;
	
		return FALSE;
	}

	template<class T> BOOL ReadType(LPCTSTR lpszKey, T& type)
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
		
			ar >> type;
			ar.Close();
				
			file.Close();
			
			delete pszData;
			return TRUE;
		}

		delete pszData;
		return FALSE;
	}

protected:
	HKEY		m_hKey;
	CString	m_sPath;

};

#endif // !defined(AFX_REGISTRY_H__08F26279_A63D_11D3_B73D_0050DA34A2BA__INCLUDED_)
