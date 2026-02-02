#if !defined(AFX_CDLGACTIONMESSAGE_H__2F3D1880_D5EE_48C1_9738_F60A88B060FE__INCLUDED_)
#define AFX_CDLGACTIONMESSAGE_H__2F3D1880_D5EE_48C1_9738_F60A88B060FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionMessage.h : header file
//
#include "..\Shared\richeditctrlex.h"
  #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


     class CAction;  //forward declaration for compiling speed

/////////////////////////////////////////////////////////////////////////////
// CDlgActionMessage dialog


class CDlgActionMessage : public CBkDialogST
{
// Construction
public:
	CDlgActionMessage(CWnd* pParent = NULL);   // standard constructor

    //I grouped these so they are easy to cut and paste into new dialogs, sue me
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;
    void CDlgActionMessage::SetGray();


    // Dialog Data
	//{{AFX_DATA(CDlgActionMessage)
	enum { IDD = IDD_ACTION_MESSAGE };
	CButtonST	m_but_token;
	BOOL	m_b_log;
	BOOL	m_b_macro;
	BOOL	m_b_popup;
	BOOL	m_b_say;
    CRichEditCtrlEx m_richedit;
	BOOL	m_b_email;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionMessage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgActionMessage)
	virtual BOOL OnInitDialog();
	afx_msg void OnFont();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEmail();
	afx_msg void OnBEmail();
	afx_msg void OnToken();
	afx_msg void OnHear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONMESSAGE_H__2F3D1880_D5EE_48C1_9738_F60A88B060FE__INCLUDED_)
