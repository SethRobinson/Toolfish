#if !defined(AFX_CDLGACTIONSOUND_H__F9EE5104_B374_4E00_895E_C63DEFC19BFD__INCLUDED_)
#define AFX_CDLGACTIONSOUND_H__F9EE5104_B374_4E00_895E_C63DEFC19BFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionSound.h : header file
//

 class CAction;  //forward declaration for compiling speed
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionSound dialog

class CDlgActionSound : public CBkDialogST
{
// Construction
public:
	CDlgActionSound(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionSound)
	enum { IDD = IDD_ACTION_SOUND };
	CString	m_cst_filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionSound)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionSound)
	afx_msg void OnBrowse();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONSOUND_H__F9EE5104_B374_4E00_895E_C63DEFC19BFD__INCLUDED_)
