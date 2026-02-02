#if !defined(AFX_CDLGACTIONEMAIL_H__C4C80178_50EC_40A8_885C_22653A66B6B2__INCLUDED_)
#define AFX_CDLGACTIONEMAIL_H__C4C80178_50EC_40A8_885C_22653A66B6B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionEmail.h : header file
//
     #include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"

class CAction;  //forward declaration for compiling speed

 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionEmail dialog

class CDlgActionEmail : public CBkDialogST
{
// Construction
public:
    void CDlgActionEmail::GrayOutStuff();
    CDlgActionEmail(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;

// Dialog Data
	//{{AFX_DATA(CDlgActionEmail)
	enum { IDD = IDD_ACTION_EMAIL };
	CString	m_cst_password;
	BOOL	m_b_say_count;
	BOOL	m_b_say_from;
	BOOL	m_b_say_subject;
	CString	m_cst_logon;
	CString	m_cst_server;
	BOOL	m_b_filter_active;
	int		m_i_filter_field;
	int		m_i_filter_have;
	CString	m_cst_filter_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionEmail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionEmail)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnFilterActive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONEMAIL_H__C4C80178_50EC_40A8_885C_22653A66B6B2__INCLUDED_)
