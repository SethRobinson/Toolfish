#if !defined(AFX_CSMPTSETUP_H__8DE78290_321D_457C_B968_19391CDE3138__INCLUDED_)
#define AFX_CSMPTSETUP_H__8DE78290_321D_457C_B968_19391CDE3138__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSMPTSetup.h : header file
//
#include "CGLobals.h"

#include "..\Shared\all.h"
#include "..\Shared\uni.h"

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CSMPTSetup dialog

class CSMPTSetup : public CBkDialogST
{
// Construction
public:
	CSMPTSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSMPTSetup)
	enum { IDD = IDD_SMTP_SETUP };
	CComboBox	m_combo;
	CString	m_cst_email;
	CString	m_cst_server;
	BOOL	m_b_logon;
	BOOL	m_b_mime;
	CString	m_cst_password;
	CString	m_cst_username;
	int		m_i_port;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSMPTSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void CSMPTSetup::BuildComboBox();
    void CSMPTSetup::SetGrayStuff();
	// Generated message map functions
	//{{AFX_MSG(CSMPTSetup)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLogon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSMPTSETUP_H__8DE78290_321D_457C_B968_19391CDE3138__INCLUDED_)
