#if !defined(AFX_CDLGTRIGGERHOTKEY_H__A32E1302_6AFF_4255_A61F_A97278E8513A__INCLUDED_)
#define AFX_CDLGTRIGGERHOTKEY_H__A32E1302_6AFF_4255_A61F_A97278E8513A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTriggerHotkey.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerHotkey dialog
class CTrigger;
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgTriggerHotkey : public CBkDialogST
{
// Construction
public:
	CDlgTriggerHotkey(CWnd* pParent = NULL);   // standard constructor
   void SetTriggerPointer(CTrigger *p_new) { m_p_action = p_new;}
    CTrigger *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgTriggerHotkey)
	enum { IDD = IDD_TRIGGER_HOTKEY };
	CComboBox	m_combo;
	BOOL	m_b_alt;
	BOOL	m_b_ctrl;
	BOOL	m_b_shift;
	BOOL	m_b_pass_through;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTriggerHotkey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTriggerHotkey)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void PopulateCombo();
    void CDlgTriggerHotkey::AddComboItem(TCHAR *p_name, int data);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTRIGGERHOTKEY_H__A32E1302_6AFF_4255_A61F_A97278E8513A__INCLUDED_)
