#if !defined(AFX_CDLGACTIONLEET_H__5F6EBB45_D39E_4615_9D38_B83D27F6C019__INCLUDED_)
#define AFX_CDLGACTIONLEET_H__5F6EBB45_D39E_4615_9D38_B83D27F6C019__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionLeet.h : header file
//
 class CAction;  //forward declaration for compiling speed
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionLeet dialog

class CDlgActionLeet : public CBkDialogST
{
// Construction
public:
	CDlgActionLeet(CWnd* pParent = NULL);   // standard constructor
  void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionLeet)
	enum { IDD = IDD_ACTION_LEET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionLeet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionLeet)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONLEET_H__5F6EBB45_D39E_4615_9D38_B83D27F6C019__INCLUDED_)
