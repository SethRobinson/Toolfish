#if !defined(AFX_CDLGACTIONREBOOT_H__FD498236_2563_4E5F_AB04_7FEE2D3688EE__INCLUDED_)
#define AFX_CDLGACTIONREBOOT_H__FD498236_2563_4E5F_AB04_7FEE2D3688EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionReboot.h : header file
//
class CAction;  //forward declaration for compiling speed

 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionReboot dialog

class CDlgActionReboot : public CBkDialogST
{
// Construction
public:
	CDlgActionReboot(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionReboot)
	enum { IDD = IDD_ACTION_REBOOT };
	CComboBox	m_combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionReboot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionReboot)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONREBOOT_H__FD498236_2563_4E5F_AB04_7FEE2D3688EE__INCLUDED_)
