#if !defined(AFX_DLGWIZARDREMINDERX_H__AB53A410_712F_4BF1_A1F5_321C67B81694__INCLUDED_)
#define AFX_DLGWIZARDREMINDERX_H__AB53A410_712F_4BF1_A1F5_321C67B81694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWizardReminderX.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"



/////////////////////////////////////////////////////////////////////////////
// CDlgWizardReminderX dialog

class CDlgWizardReminderX : public CBkDialogST
{
// Construction
public:
	CDlgWizardReminderX(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWizardReminderX)
	enum { IDD = IDD_WIZARD_ONE_TIME_REMINDER_X };
	BOOL	m_b_popup;
	BOOL	m_b_say;
	CString	m_cst_text;
	//}}AFX_DATA

     int Init(int i_time);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardReminderX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    
    int m_delay_seconds; 

	// Generated message map functions
	//{{AFX_MSG(CDlgWizardReminderX)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnHear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWIZARDREMINDERX_H__AB53A410_712F_4BF1_A1F5_321C67B81694__INCLUDED_)
