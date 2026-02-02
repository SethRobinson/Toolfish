#if !defined(AFX_CDLGWIZARD_H__1311D94B_3A6D_474F_8A56_A3818C140EE8__INCLUDED_)
#define AFX_CDLGWIZARD_H__1311D94B_3A6D_474F_8A56_A3818C140EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgWizard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWizard dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

class CDlgWizard : public CBkDialogST
{
// Construction
public:
	CDlgWizard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWizard)
	enum { IDD = IDD_WIZARD };
	CButtonST	m_but_pic;
	BOOL	m_b_pop_up;
	BOOL	m_b_say_it;
	CString	m_st_filename;
	CString	m_st_content_percent;
	CString	m_st_check_time;
	BOOL	m_b_log_it;
	BOOL	m_b_play_sound;
	CString	m_cst_say;
	BOOL	m_b_browser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizard)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTest();
	virtual void OnOK();
	afx_msg void OnSayIt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGWIZARD_H__1311D94B_3A6D_474F_8A56_A3818C140EE8__INCLUDED_)
