#if !defined(AFX_CDLGACTIONWEBSITE_H__9775B13F_C86D_4714_A338_0E5626560756__INCLUDED_)
#define AFX_CDLGACTIONWEBSITE_H__9775B13F_C86D_4714_A338_0E5626560756__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionWebsite.h : header file
  #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"



class CAction;
/////////////////////////////////////////////////////////////////////////////
// CDlgActionWebsite dialog

class CDlgActionWebsite : public CBkDialogST
{
// Construction
public:
	CDlgActionWebsite(CWnd* pParent = NULL);   // standard constructor
   //I grouped these so they are easy to cut and paste into new dialogs, sue me
    void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionWebsite)
	enum { IDD = IDD_ACTION_WEBSITE };
	CString	m_filename;
	BOOL	m_b_new;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionWebsite)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionWebsite)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTest();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONWEBSITE_H__9775B13F_C86D_4714_A338_0E5626560756__INCLUDED_)
