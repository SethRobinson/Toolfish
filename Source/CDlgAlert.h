#if !defined(AFX_CDLGALERT_H__14021B8B_6C9B_41A9_B6B0_4212677CB876__INCLUDED_)
#define AFX_CDLGALERT_H__14021B8B_6C9B_41A9_B6B0_4212677CB876__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgAlert.h : header file
//
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
#include "..\Shared\richeditctrlex.h"

class CAction;

/////////////////////////////////////////////////////////////////////////////
// CDlgAlert dialog

class CDlgAlert : public CBkDialogST
{
// Construction
public:
	CDlgAlert(CWnd* pParent = NULL);   // standard constructor
    void CDlgAlert::StoreTextPointerForLater(const TCHAR *p_ws_text);
  void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;
    CString m_cst_window_caption;
    void SetWindowCaption(CString cst_new){m_cst_window_caption = cst_new;}
    // Dialog Data
	//{{AFX_DATA(CDlgAlert)
	enum { IDD = IDD_ALERT };
       CRichEditCtrlEx m_richedit;

    //}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlert)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        void SetText(WCHAR *p_ws_text);
 
        TCHAR *m_p_ws_text;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGALERT_H__14021B8B_6C9B_41A9_B6B0_4212677CB876__INCLUDED_)
