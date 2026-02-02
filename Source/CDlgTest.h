#if !defined(AFX_CDLGTEST_H__DCA5AA09_47F9_4BE9_B1C6_C254D380F2CC__INCLUDED_)
#define AFX_CDLGTEST_H__DCA5AA09_47F9_4BE9_B1C6_C254D380F2CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTest.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
#include "CAction.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTest dialog

class CDlgTest : public CBkDialogST
{
// Construction
public:
	CDlgTest(CWnd* pParent = NULL);   // standard constructor
    TCHAR m_wst_url[C_MAX_URL_SIZE];
    void GetPage();
    
    
    void SetAuthentication(bool b_authenticate, CString cst_logon, CString cst_pass )
    {
        m_b_authenticate = b_authenticate;
        m_cst_logon = cst_logon;
        m_cst_pass = cst_pass;
    }
    // Dialog Data
	//{{AFX_DATA(CDlgTest)
	enum { IDD = IDD_TEST };
	CString	m_cst_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTest)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnHtml();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    bool m_b_authenticate;
    CString m_cst_logon;
    CString m_cst_pass;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTEST_H__DCA5AA09_47F9_4BE9_B1C6_C254D380F2CC__INCLUDED_)
