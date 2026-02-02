#if !defined(AFX_CDLGEVENT_H__BE782C88_E150_460B_AA2E_397E75CC70BB__INCLUDED_)
#define AFX_CDLGEVENT_H__BE782C88_E150_460B_AA2E_397E75CC70BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgEvent.h : header file
//

class CEvent; //forward for compile speed

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEvent dialog

class CDlgEvent : public CBkDialogST

{
// Construction
public:
	CDlgEvent(CWnd* pParent = NULL);   // standard constructor
    void SetEventToEdit(CEvent *p_new);
    void CDlgEvent::SetName();
// Dialog Data
	//{{AFX_DATA(CDlgEvent)
	enum { IDD = IDD_EVENT };
	CButtonST	m_but_action_up;
	CButtonST	m_but_action_down;
	CButtonST	m_but_trigger_up;
	CButtonST	m_but_down;
	CListBox	m_list_action;
	CListBox	m_list_trigger;
	CString	m_cst_name;
	BOOL	m_b_enabled;
	BOOL	m_b_auto_name;
	CString	m_cst_filename;
	BOOL	m_b_quiet;
	BOOL	m_b_auto_delete_event_after_running;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   
    void RebuildActionList();
    void RebuildTriggerList();
    void CalculateNextRun();
 
 
    CEvent *m_p_event;
	// Generated message map functions
	//{{AFX_MSG(CDlgEvent)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusName();
	afx_msg void OnTriggerAdd();
	afx_msg void OnTriggerDelete();
	afx_msg void OnTriggerEdit();
	afx_msg void OnDblclkTriggerList();
	afx_msg void OnEnabled();
	virtual void OnOK();
	afx_msg void OnActionAdd();
	afx_msg void OnActionDelete();
	afx_msg void OnActionEdit();
	afx_msg void OnDblclkActionList();
	afx_msg void OnEventName();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTriggerDown();
	afx_msg void OnActionUp();
	afx_msg void OnActionDown();
	afx_msg void OnTriggerUp();
	afx_msg void OnRename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGEVENT_H__BE782C88_E150_460B_AA2E_397E75CC70BB__INCLUDED_)
