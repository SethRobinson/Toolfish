#if !defined(AFX_CDLGACTIONTOGGLE_H__7E5D8841_094B_4CA1_BF61_B26FFD1156FE__INCLUDED_)
#define AFX_CDLGACTIONTOGGLE_H__7E5D8841_094B_4CA1_BF61_B26FFD1156FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionToggle.h : header file
//
 class CAction;  //forward declaration for compiling speed
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionToggle dialog

class CDlgActionToggle : public CBkDialogST
{
// Construction
public:
	CDlgActionToggle(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionToggle)
	enum { IDD = IDD_ACTION_TOGGLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionToggle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionToggle)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONTOGGLE_H__7E5D8841_094B_4CA1_BF61_B26FFD1156FE__INCLUDED_)
