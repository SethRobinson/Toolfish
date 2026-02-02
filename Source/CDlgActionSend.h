#if !defined(AFX_CDLGACTIONSEND_H__C5164E35_DCC9_4E04_9E1A_427C62AD17EB__INCLUDED_)
#define AFX_CDLGACTIONSEND_H__C5164E35_DCC9_4E04_9E1A_427C62AD17EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionSend.h : header file
//
 class CAction;  //forward declaration for compiling speed

 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionSend dialog

class CDlgActionSend : public CBkDialogST
{
// Construction
public:
	CDlgActionSend(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;
    int CDlgActionSend::AddMessage(TCHAR * p_t_text, int i_data_code);

// Dialog Data
	//{{AFX_DATA(CDlgActionSend)
	enum { IDD = IDD_ACTION_SEND };
	CButtonST	m_but_view;
	CComboBox	m_combo_key;
	CComboBox	m_combo_message;
	CComboBox	m_combo_filename;
	CString	m_cst_search;
	BOOL	m_b_alt;
	BOOL	m_b_ctrl;
	BOOL	m_b_shift;
	DWORD	m_dw_lparam;
	DWORD	m_dw_wparam;
	UINT	m_ui_msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionSend)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void CDlgActionSend::AddComboItem(TCHAR *p_name, int data);
    void CDlgActionSend::GrayOutStuff();

    // Generated message map functions
	//{{AFX_MSG(CDlgActionSend)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONSEND_H__C5164E35_DCC9_4E04_9E1A_427C62AD17EB__INCLUDED_)
