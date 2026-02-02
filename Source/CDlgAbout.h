#if !defined(AFX_CDLGABOUT_H__08B23A42_8348_4CAD_A383_3370AB986D2F__INCLUDED_)
#define AFX_CDLGABOUT_H__08B23A42_8348_4CAD_A383_3370AB986D2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgAbout.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog

class CDlgAbout : public CBkDialogST
{
// Construction
public:
	CDlgAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAbout)
	enum { IDD = IDD_ABOUT };
	CButtonST	m_but_toolfish_help;
	CButtonST	m_but_rtsoft_logo;
	CButtonST	m_but_logo;
	CButtonST	m_but_rtsoft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAbout)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnToolfishHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGABOUT_H__08B23A42_8348_4CAD_A383_3370AB986D2F__INCLUDED_)
