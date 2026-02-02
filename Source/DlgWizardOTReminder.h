#if !defined(AFX_DLGWIZARDOTREMINDER_H__4E384E21_58A6_4DBA_AB27_6EED65024652__INCLUDED_)
#define AFX_DLGWIZARDOTREMINDER_H__4E384E21_58A6_4DBA_AB27_6EED65024652__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWizardOTReminder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardOTReminder dialog

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgWizardOTReminder : public CBkDialogST
{
// Construction
public:
	CDlgWizardOTReminder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWizardOTReminder)
	enum { IDD = IDD_WIZARD_ONE_TIME_REMINDER };
	CDateTimeCtrl	m_time;
	CMonthCalCtrl	m_date;
	CButtonST	m_but_pic;
	CString	m_cst_text;
	BOOL	m_b_popup;
	BOOL	m_b_say;
	BOOL	m_b_disable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardOTReminder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizardOTReminder)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWIZARDOTREMINDER_H__4E384E21_58A6_4DBA_AB27_6EED65024652__INCLUDED_)
