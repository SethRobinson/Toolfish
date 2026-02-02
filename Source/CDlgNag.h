#if !defined(AFX_CDLGNAG_H__791CC739_5088_447E_9707_ACFFBEB5F6C9__INCLUDED_)
#define AFX_CDLGNAG_H__791CC739_5088_447E_9707_ACFFBEB5F6C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgNag.h : header file
//

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

////////////////////////////////////////////////////////////////////////////
// CDlgNag dialog

class CDlgNag : public CBkDialogST
{
// Construction
public:
	CDlgNag(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNag)
	enum { IDD = IDD_NAG };
	CButtonST	m_but_logo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNag)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNag)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnPurchase();
	afx_msg void OnEnterCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGNAG_H__791CC739_5088_447E_9707_ACFFBEB5F6C9__INCLUDED_)
