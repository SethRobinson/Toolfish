// Source.h : main header file for the SOURCE application
//


#if !defined(AFX_SOURCE_H__96C65F7F_2944_459E_901C_E848FFC3839C__INCLUDED_)
#define AFX_SOURCE_H__96C65F7F_2944_459E_901C_E848FFC3839C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSource:
// See Source.cpp for the implementation of this class
//

class CSource : public CWinApp
{
public:
	CSource();
    void WinHelp( DWORD dwData, UINT nCmd );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSource)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSource)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOURCE_H__96C65F7F_2944_459E_901C_E848FFC3839C__INCLUDED_)
