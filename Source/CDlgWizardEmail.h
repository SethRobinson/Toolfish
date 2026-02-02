#if !defined(AFX_CDLGWIZARDEMAIL_H__B026371E_80A2_4C49_A71E_80463588BB79__INCLUDED_)
#define AFX_CDLGWIZARDEMAIL_H__B026371E_80A2_4C49_A71E_80463588BB79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgWizardEmail.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgWizardEmail dialog

class CDlgWizardEmail : public CBkDialogST
{
// Construction
public:
	CDlgWizardEmail(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWizardEmail)
	enum { IDD = IDD_WIZARD_EMAIL };
	CComboBox	m_combo;
	CString	m_cst_logon;
	CString	m_cst_password;
	CString	m_cst_server;
	BOOL	m_b_say_count;
	BOOL	m_b_say_from;
	BOOL	m_b_say_subject;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardEmail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizardEmail)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGWIZARDEMAIL_H__B026371E_80A2_4C49_A71E_80463588BB79__INCLUDED_)
