#if !defined(AFX_CDLGWIZARDRECURRENTREMINDER_H__5BB1407B_C18C_4C13_86EE_082F7E68EA46__INCLUDED_)
#define AFX_CDLGWIZARDRECURRENTREMINDER_H__5BB1407B_C18C_4C13_86EE_082F7E68EA46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgWizardRecurrentReminder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardRecurrentReminder dialog
 
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgWizardRecurrentReminder : public CBkDialogST
{
// Construction
public:
	CDlgWizardRecurrentReminder(CWnd* pParent = NULL);   // standard constructor
    void InitComboBox();
    void CDlgWizardRecurrentReminder::SetGray();
// Dialog Data
	//{{AFX_DATA(CDlgWizardRecurrentReminder)
	enum { IDD = IDD_WIZARD_RECURRENT_REMINDER };
	CComboBox	m_combo;
	CButtonST	m_but_pic;
	CDateTimeCtrl	m_time;
	CMonthCalCtrl	m_date;
	BOOL	m_b_popup;
	BOOL	m_b_say;
	BOOL	m_b_0;
	BOOL	m_b_1;
	BOOL	m_b_2;
	BOOL	m_b_3;
	BOOL	m_b_4;
	BOOL	m_b_5;
	BOOL	m_b_6;
	CString	m_cst_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardRecurrentReminder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizardRecurrentReminder)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHear();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGWIZARDRECURRENTREMINDER_H__5BB1407B_C18C_4C13_86EE_082F7E68EA46__INCLUDED_)
