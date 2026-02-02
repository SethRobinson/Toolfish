#if !defined(AFX_CDLGMAILVIEW_H__C7676DF1_6422_42AB_9B76_072B6B90B219__INCLUDED_)
#define AFX_CDLGMAILVIEW_H__C7676DF1_6422_42AB_9B76_072B6B90B219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgMailView.h : header file
//
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
#include "..\Shared\richeditctrlex.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMailView dialog

class CEvent;

class CDlgMailView : public CBkDialogST
{
// Construction
public:
	CDlgMailView(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMailView)
	enum { IDD = IDD_MAIL_VIEW };
	    CRichEditCtrlEx m_richedit;

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMailView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	void CDlgMailView::BuildText();
    void CDlgMailView::AddLineOfText(WCHAR *p_w_text);
    void CDlgMailView::ProcessMailBox(CEvent *p_event, int i_action);
    // Generated message map functions
	//{{AFX_MSG(CDlgMailView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGMAILVIEW_H__C7676DF1_6422_42AB_9B76_072B6B90B219__INCLUDED_)
