// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AD0AEFE4_DD89_4EF2_83A5_C5A91F3B2D91__INCLUDED_)
#define AFX_STDAFX_H__AD0AEFE4_DD89_4EF2_83A5_C5A91F3B2D91__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <stdlib.h>

#ifdef _DEBUG
    #ifndef CRTDBG_MAP_ALLOC
    #define CRTDBG_MAP_ALLOC
    #endif
#include <crtdbg.h>
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxsock.h>		// MFC socket extensions
//#include <winsock.h>
#include <afxtempl.h>		
#include <afxpriv.h>		


// Commonly used project headers (for PCH optimization)
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\std_all.h"

#include "resource.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__AD0AEFE4_DD89_4EF2_83A5_C5A91F3B2D91__INCLUDED_)
