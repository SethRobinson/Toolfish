#if !defined(AFX_CDLGINTRO_H__2400151F_8D7A_4C2D_BBD4_D899C78A3563__INCLUDED_)
#define AFX_CDLGINTRO_H__2400151F_8D7A_4C2D_BBD4_D899C78A3563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgIntro.h : header file
//
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgIntro dialog

class CDlgIntro : public CBkDialogST
{
// Construction
public:
	CDlgIntro(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIntro)
	enum { IDD = IDD_INTRO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIntro)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIntro)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGINTRO_H__2400151F_8D7A_4C2D_BBD4_D899C78A3563__INCLUDED_)
