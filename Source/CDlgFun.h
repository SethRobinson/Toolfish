#if !defined(AFX_CDLGFUN_H__3BE0ACCB_A7DB_436F_878A_B72FDE6BF28E__INCLUDED_)
#define AFX_CDLGFUN_H__3BE0ACCB_A7DB_436F_878A_B72FDE6BF28E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgFun.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFun dialog

class CDlgFun : public CBkDialogST
{
// Construction
public:
	CDlgFun(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFun)
	enum { IDD = IDD_FUN };
	BOOL	m_b_overlay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFun)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnStatistics();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGFUN_H__3BE0ACCB_A7DB_436F_878A_B72FDE6BF28E__INCLUDED_)
