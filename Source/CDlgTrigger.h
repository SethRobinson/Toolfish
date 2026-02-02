#if !defined(AFX_CDLGTRIGGER_H__FD63B437_3B4F_465A_A2C5_6AD7260D8B6E__INCLUDED_)
#define AFX_CDLGTRIGGER_H__FD63B437_3B4F_465A_A2C5_6AD7260D8B6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTrigger.h : header file
//
class CTrigger; //forward for speed
/////////////////////////////////////////////////////////////////////////////
// CDlgTrigger dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgTrigger : public CBkDialogST
{
// Construction
public:
	CDlgTrigger(CWnd* pParent = NULL);   // standard constructor
    void SetTriggerToEdit(CTrigger *p_new);
    CTrigger * m_p_trigger;

// Dialog Data
	//{{AFX_DATA(CDlgTrigger)
	enum { IDD = IDD_TRIGGER };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTrigger)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTrigger)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTRIGGER_H__FD63B437_3B4F_465A_A2C5_6AD7260D8B6E__INCLUDED_)
