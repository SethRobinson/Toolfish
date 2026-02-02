#if !defined(AFX_CDLGKEYLOG_H__67DBBEE9_9291_4F92_9625_7167BA82EAAF__INCLUDED_)
#define AFX_CDLGKEYLOG_H__67DBBEE9_9291_4F92_9625_7167BA82EAAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgKeyLog.h : header file
//
  #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgKeyLog dialog

class CDlgKeyLog : public CBkDialogST
{
// Construction
public:
	CDlgKeyLog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgKeyLog)
	enum { IDD = IDD_KEY_LOG };
	BOOL	m_b_log_active;
	CString	m_cst_file;
	BOOL	m_b_prog_log;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgKeyLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void CDlgKeyLog::GrayOutStuff();

	// Generated message map functions
	//{{AFX_MSG(CDlgKeyLog)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogBrowse();
	afx_msg void OnViewLog();
	virtual void OnOK();
	afx_msg void OnBKeyLogActive();
	afx_msg void OnDeleteLog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGKEYLOG_H__67DBBEE9_9291_4F92_9625_7167BA82EAAF__INCLUDED_)
