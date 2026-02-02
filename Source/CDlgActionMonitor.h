#if !defined(AFX_CDLGACTIONMONITOR_H__3C8F918E_8FB8_4932_9AFC_4A33BAC55784__INCLUDED_)
#define AFX_CDLGACTIONMONITOR_H__3C8F918E_8FB8_4932_9AFC_4A33BAC55784__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionMonitor.h : header file
//
 class CAction;
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionMonitor dialog

class CDlgActionMonitor : public CBkDialogST
{
// Construction
public:
	CDlgActionMonitor(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionMonitor)
	enum { IDD = IDD_ACTION_MONITOR };
	BOOL	m_b_error;
	CString	m_cst_filename;
	int		m_i_port;
	int		m_i_size;
	BOOL	m_b_timeout;
	BOOL	m_b_webpage;
	BOOL	m_b_size;
	BOOL	m_b_reverse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    void CDlgActionMonitor::SetGrayStuff();
    void CDlgActionMonitor::UpdateStats();

	// Generated message map functions
	//{{AFX_MSG(CDlgActionMonitor)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnWebpage();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONMONITOR_H__3C8F918E_8FB8_4932_9AFC_4A33BAC55784__INCLUDED_)
