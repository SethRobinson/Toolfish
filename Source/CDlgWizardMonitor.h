#if !defined(AFX_CDLGWIZARDMONITOR_H__4D591A59_47E2_4740_90FF_5BE4B832C86A__INCLUDED_)
#define AFX_CDLGWIZARDMONITOR_H__4D591A59_47E2_4740_90FF_5BE4B832C86A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgWizardMonitor.h : header file
//
// CDlgWizard dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgWizardMonitor dialog

class CDlgWizardMonitor : public CBkDialogST
{
// Construction
public:
	CDlgWizardMonitor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWizardMonitor)
	enum { IDD = IDD_WIZARD_MONITOR };
	CComboBox	m_combo;
	BOOL	m_b_email;
	CString	m_cst_email_from;
	CString	m_cst_email_to;
	CString	m_cst_filename;
	BOOL	m_b_popup;
	int		m_i_port;
	BOOL	m_b_quiet;
	BOOL	m_b_say;
	BOOL	m_b_webpage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void GrayOutStuff();
    // Generated message map functions
	//{{AFX_MSG(CDlgWizardMonitor)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSmtp();
	afx_msg void OnEmail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGWIZARDMONITOR_H__4D591A59_47E2_4740_90FF_5BE4B832C86A__INCLUDED_)
