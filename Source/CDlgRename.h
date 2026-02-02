#if !defined(AFX_CDLGRENAME_H__C50CCB5F_DB53_4B0A_8F4B_D60AA7458764__INCLUDED_)
#define AFX_CDLGRENAME_H__C50CCB5F_DB53_4B0A_8F4B_D60AA7458764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgRename.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgRename dialog

class CDlgRename : public CBkDialogST
{
// Construction
 
public:
	CDlgRename(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRename)
	enum { IDD = IDD_RENAME };
	//}}AFX_DATA
    CString	m_cst_filename;



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRename)
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGRENAME_H__C50CCB5F_DB53_4B0A_8F4B_D60AA7458764__INCLUDED_)
