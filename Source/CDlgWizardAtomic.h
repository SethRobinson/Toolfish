#if !defined(AFX_CDLGWIZARDATOMIC_H__3A209448_C0D8_485A_928A_D8BAB15DA57C__INCLUDED_)
#define AFX_CDLGWIZARDATOMIC_H__3A209448_C0D8_485A_928A_D8BAB15DA57C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgWizardAtomic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWizardAtomic dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"



class CDlgWizardAtomic : public CBkDialogST
{
// Construction
public:
	CDlgWizardAtomic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWizardAtomic)
	enum { IDD = IDD_WIZARD_ATOMIC };
	CComboBox	m_combo_time;
	CComboBox	m_combo;
	CString	m_cst_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWizardAtomic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWizardAtomic)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGWIZARDATOMIC_H__3A209448_C0D8_485A_928A_D8BAB15DA57C__INCLUDED_)
