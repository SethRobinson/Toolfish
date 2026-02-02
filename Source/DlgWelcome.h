#if !defined(AFX_DLGWELCOME_H__72982C08_E717_4B3E_9DBC_3C4C9A5127F9__INCLUDED_)
#define AFX_DLGWELCOME_H__72982C08_E717_4B3E_9DBC_3C4C9A5127F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWelcome.h : header file
//
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgWelcome dialog

class CDlgWelcome : public CBkDialogST
{
// Construction
public:
	CDlgWelcome(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWelcome)
	enum { IDD = IDD_WELCOME };
	CButtonST	m_but_tip4;
	CButtonST	m_but_tip3;
	CButtonST	m_but_tip2;
	CButtonST	m_but_tip1;
	CButtonST	m_but_pic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWelcome)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWelcome)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWELCOME_H__72982C08_E717_4B3E_9DBC_3C4C9A5127F9__INCLUDED_)
