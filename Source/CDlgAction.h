#if !defined(AFX_CDLGACTION_H__C322F636_62A8_41A6_B3E2_11211433BE60__INCLUDED_)
#define AFX_CDLGACTION_H__C322F636_62A8_41A6_B3E2_11211433BE60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgAction.h : header file
//
class CAction;

/////////////////////////////////////////////////////////////////////////////
// CDlgAction dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgAction : public CBkDialogST
{
// Construction
public:
	CDlgAction(CWnd* pParent = NULL);   // standard constructor
   void CDlgAction::SetActionToEdit(CAction *p_new);

// Dialog Data
	//{{AFX_DATA(CDlgAction)
	enum { IDD = IDD_ACTION };
	CListBox	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CAction *m_p_action;
	// Generated message map functions
	//{{AFX_MSG(CDlgAction)
	virtual void OnOK();
	afx_msg void OnDblclkList();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTION_H__C322F636_62A8_41A6_B3E2_11211433BE60__INCLUDED_)
