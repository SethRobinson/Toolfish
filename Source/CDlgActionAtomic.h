#if !defined(AFX_CDLGACTIONATOMIC_H__D7FF3BCF_168E_4C51_8353_0C205A5E6463__INCLUDED_)
#define AFX_CDLGACTIONATOMIC_H__D7FF3BCF_168E_4C51_8353_0C205A5E6463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionAtomic.h : header file
//
class CAction;  //forward declaration for compiling speed
  
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionAtomic dialog
  
void InitAtomicComboBox(CComboBox &combo);

class CDlgActionAtomic : public CBkDialogST
{
// Construction
public:
	CDlgActionAtomic(CWnd* pParent = NULL);   // standard constructor
       void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionAtomic)
	enum { IDD = IDD_ACTION_ATOMIC };
	CComboBox	m_combo;
	CString	m_cst_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionAtomic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionAtomic)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONATOMIC_H__D7FF3BCF_168E_4C51_8353_0C205A5E6463__INCLUDED_)
