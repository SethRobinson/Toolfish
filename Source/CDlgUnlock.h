#if !defined(AFX_CDLGUNLOCK_H__98FCEEA0_3FC0_4F0B_861A_04B14E73DCE8__INCLUDED_)
#define AFX_CDLGUNLOCK_H__98FCEEA0_3FC0_4F0B_861A_04B14E73DCE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgUnlock.h : header file
//

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"



/////////////////////////////////////////////////////////////////////////////
// CDlgUnlock dialog

class CDlgUnlock : public CBkDialogST
{
// Construction
public:
	CDlgUnlock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUnlock)
	enum { IDD = IDD_UNLOCK };
	CString	m_ws_key;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUnlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUnlock)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGUNLOCK_H__98FCEEA0_3FC0_4F0B_861A_04B14E73DCE8__INCLUDED_)
