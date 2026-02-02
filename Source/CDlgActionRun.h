#if !defined(AFX_CDLGACTIONRUN_H__71C1ECE7_8C19_4B5B_AEA6_226A00982FC9__INCLUDED_)
#define AFX_CDLGACTIONRUN_H__71C1ECE7_8C19_4B5B_AEA6_226A00982FC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionRun.h : header file
class CAction;  //forward declaration for compiling speed

 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgActionRun dialog

class CDlgActionRun : public CBkDialogST
{
// Construction
public:
	CDlgActionRun(CWnd* pParent = NULL);   // standard constructor
     void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;

// Dialog Data
	//{{AFX_DATA(CDlgActionRun)
	enum { IDD = IDD_ACTION_RUN };
	CString	m_cst_filename;
	CString	m_edit_parms;
	BOOL	m_b_wait;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionRun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionRun)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONRUN_H__71C1ECE7_8C19_4B5B_AEA6_226A00982FC9__INCLUDED_)
