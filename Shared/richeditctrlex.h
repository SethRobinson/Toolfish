//******************************************************************************
//******************************************************************************
// Owner:   Andrew Forget (AndrewFo,x3438) 
//          Copyright © 1999, SOFTIMAGE, Avid Technologies
//------------------------------------------------------------------------------
//
// @doc     RICHEDITCTRLEX
//
// @module  RichEditCtrlEx.h |
//          This file declares the rich edit control extended class
//          <c CRichEditCtrlEx>.
//
// @version
//------------------------------------------------------------------------------
//.Version: Date:       Author:     Comments:
//.-------- ----------- ----------- --------------------------------------------
//.  1.0    01/18/99    AndrewFo    New file.
//******************************************************************************
//******************************************************************************

#include <afxcmn.h>

#ifndef __RICHEDITCTRLEX_H_RICHEDITCTRLEX_C5C1A145_AF02_11D2_8084_00A0C96E63E1
#define __RICHEDITCTRLEX_H_RICHEDITCTRLEX_C5C1A146_AF02_11D2_8084_00A0C96E63E1

#if _MSC_VER >= 1000
#pragma once
#endif


//******************************************************************************
// Owner:   Andrew Forget (AndrewFo,x3438) 
//------------------------------------------------------------------------------
//
// @class   _AFX_RICHEDITEX_STATE |
//          This is the rich edit control extended state class.
//
// @version
//------------------------------------------------------------------------------
//.Version: Date:       Author:     Comments:
//.-------- ----------- ----------- --------------------------------------------
//.  1.0    01/18/99    AndrewFo    New code.
//******************************************************************************

class _AFX_RICHEDITEX_STATE
{
	public:

			 _AFX_RICHEDITEX_STATE() ;
	virtual ~_AFX_RICHEDITEX_STATE() ;

	public:

	HINSTANCE m_hInstRichEdit20 ;
} ;


//******************************************************************************
//
// Function Prototype
//
//******************************************************************************

BOOL PASCAL AfxInitRichEditEx() ;


//******************************************************************************
// Owner:   Andrew Forget (AndrewFo,x3438) 
//------------------------------------------------------------------------------
//
// @class   CRichEditCtrlEx |
//          This is the rich edit control extended class which used RichEdit
//          v2.0 control.
//
// @base    public | CRichEditCtrl
//
// @version
//------------------------------------------------------------------------------
//.Version: Date:       Author:     Comments:
//.-------- ----------- ----------- --------------------------------------------
//.  1.0    01/18/99    AndrewFo    New code.
//******************************************************************************

class CRichEditCtrlEx 
	: public CRichEditCtrl
{
	////////////////////////////////////////////////////////////////////////////
	// Construction
	////////////////////////////////////////////////////////////////////////////

	public:

			 CRichEditCtrlEx() ;
	virtual ~CRichEditCtrlEx() ;

	////////////////////////////////////////////////////////////////////////////
	// Operations
	////////////////////////////////////////////////////////////////////////////

	public:

	virtual BOOL AutoURLDetect( BOOL in_fEnable ) ;

	////////////////////////////////////////////////////////////////////////////
	// ClassWizard generated virtual function overrides
	////////////////////////////////////////////////////////////////////////////

	//{{AFX_VIRTUAL( CRichEditCtrlEx )
	public:
	virtual BOOL Create( DWORD in_dwStyle, const RECT& in_rcRect, CWnd* in_pParentWnd, UINT in_nID ) ;
	//}}AFX_VIRTUAL

	////////////////////////////////////////////////////////////////////////////
	// Generated message map functions
	////////////////////////////////////////////////////////////////////////////

	protected:

	//{{AFX_MSG( CRichEditCtrlEx )
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuCopy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
} ;


//******************************************************************************
// Owner:   Andrew Forget (AndrewFo,x3438) 
//------------------------------------------------------------------------------
//
// @mfunc   BOOL | CRichEditCtrlEx | AutoURLDetect |
//          This method sets the URL auto detection to enabled or disabled.
//
// @syntax  inline BOOL AutoURLDetect( 
//						BOOL in_fEnable ) ;
//
// @parm    BOOL | in_fEnable |
//          TRUE to enable URL auto detection.
//
// @rdesc   BOOL : A boolean value indicating:
// @flag        TRUE  | Success.
// @flag        FALSE | Failure.
//
// @version
//------------------------------------------------------------------------------
//.Version: Date:       Author:     Comments:
//.-------- ----------- ----------- --------------------------------------------
//.  1.0    01/18/99    AndrewFo    New code.
//******************************************************************************

inline BOOL CRichEditCtrlEx::AutoURLDetect( BOOL in_fEnable )
	{ ASSERT(::IsWindow( m_hWnd ) ) ; return ( BOOL )::SendMessage( m_hWnd, EM_AUTOURLDETECT, in_fEnable, 0 ) ; }


//{{AFX_INSERT_LOCATION}}

#endif
