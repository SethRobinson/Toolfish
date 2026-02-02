#if !defined(AFX_CDLGTRIGGERPROCESS_H__BD2C8D65_4FCF_4E33_BF91_94CED74F6D07__INCLUDED_)
#define AFX_CDLGTRIGGERPROCESS_H__BD2C8D65_4FCF_4E33_BF91_94CED74F6D07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTriggerProcess.h : header file
//
class CTrigger; //forward for speed
/////////////////////////////////////////////////////////////////////////////
// CDlgTrigger dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerProcess dialog

class CDlgTriggerProcess : public CBkDialogST
{
// Construction
public:
	CDlgTriggerProcess(CWnd* pParent = NULL);   // standard constructor
    CTrigger * m_p_trigger;
    void SetTriggerPointer(CTrigger *p_new) { m_p_trigger = p_new;}


// Dialog Data
	//{{AFX_DATA(CDlgTriggerProcess)
	enum { IDD = IDD_TRIGGER_PROCESS };
	CButtonST	m_but_view;
	CComboBox	m_combo_filename;
	CComboBox	m_combo_found;
	CString	m_cst_search;
	BOOL	m_b_focus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTriggerProcess)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTriggerProcess)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTRIGGERPROCESS_H__BD2C8D65_4FCF_4E33_BF91_94CED74F6D07__INCLUDED_)
