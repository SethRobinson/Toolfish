#if !defined(AFX_DLGACTIONSTEALTH_H__9CF8BF23_0C2D_48E0_A4B0_737504FCFE50__INCLUDED_)
#define AFX_DLGACTIONSTEALTH_H__9CF8BF23_0C2D_48E0_A4B0_737504FCFE50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgActionStealth.h : header file
//
 class CAction;  //forward declaration for compiling speed
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgActionStealth dialog

class CDlgActionStealth : public CBkDialogST
{
// Construction
public:
	CDlgActionStealth(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionStealth)
	enum { IDD = IDD_ACTION_STEALTH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionStealth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionStealth)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACTIONSTEALTH_H__9CF8BF23_0C2D_48E0_A4B0_737504FCFE50__INCLUDED_)
