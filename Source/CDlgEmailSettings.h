#if !defined(AFX_CDLGEMAILSETTINGS_H__E4F059F1_D1BD_4CD9_9666_C20F7F881996__INCLUDED_)
#define AFX_CDLGEMAILSETTINGS_H__E4F059F1_D1BD_4CD9_9666_C20F7F881996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgEmailSettings.h : header file
//
class CAction;
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"



/////////////////////////////////////////////////////////////////////////////
// DlgEmailSettings dialog

class DlgEmailSettings : public CBkDialogST
{
// Construction
public:
	DlgEmailSettings(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(DlgEmailSettings)
	enum { IDD = IDD_EMAIL_SETTINGS };
	CString	m_cst_subject;
	CString	m_cst_from;
	CString	m_cst_to;
	CString	m_cst_mail_file;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgEmailSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgEmailSettings)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditSmtp();
	afx_msg void OnFindFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGEMAILSETTINGS_H__E4F059F1_D1BD_4CD9_9666_C20F7F881996__INCLUDED_)
