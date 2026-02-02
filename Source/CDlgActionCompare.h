#if !defined(AFX_CDLGACTIONCOMPARE_H__6D205AFE_810F_4555_9297_8B558BFE51D7__INCLUDED_)
#define AFX_CDLGACTIONCOMPARE_H__6D205AFE_810F_4555_9297_8B558BFE51D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionCompare.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgActionCompare dialog

class CAction;
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgActionCompare : public CBkDialogST
{
// Construction
public:
	CDlgActionCompare(CWnd* pParent = NULL);   // standard constructor
 //I grouped these so they are easy to cut and paste into new dialogs, sue me
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;
    void ProcessGray();


// Dialog Data
	//{{AFX_DATA(CDlgActionCompare)
	enum { IDD = IDD_ACTION_COMPARE };
	CString	m_filename;
	BOOL	m_b_content;
	BOOL	m_b_date;
	BOOL	m_b_words_found;
	BOOL	m_b_words_not_found;
	CString	m_st_content_percent;
	CString	m_st_words_found;
	CString	m_st_words_not_found;
	BOOL	m_authentication;
	CString	m_st_auth_logon;
	CString	m_st_auth_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionCompare)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionCompare)
	virtual BOOL OnInitDialog();
	afx_msg void OnTest();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAuthentication();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONCOMPARE_H__6D205AFE_810F_4555_9297_8B558BFE51D7__INCLUDED_)
