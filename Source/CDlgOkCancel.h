#if !defined(AFX_CDLGOKCANCEL_H__22C1305D_DB26_4D28_9513_BD9F54502578__INCLUDED_)
#define AFX_CDLGOKCANCEL_H__22C1305D_DB26_4D28_9513_BD9F54502578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgOkCancel.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// DlgOkCancel dialog

class DlgOkCancel : public CBkDialogST
{
// Construction
public:
	DlgOkCancel(CWnd* pParent = NULL);   // standard constructor
   void DlgOkCancel::SetTitle(TCHAR * p_message);
  void DlgOkCancel::SetDialogText(TCHAR * p_message);

// Dialog Data
	//{{AFX_DATA(DlgOkCancel)
	enum { IDD = IDD_OKCANCEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgOkCancel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString m_cst_title;
   CString m_cst_dialog;
	// Generated message map functions
	//{{AFX_MSG(DlgOkCancel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGOKCANCEL_H__22C1305D_DB26_4D28_9513_BD9F54502578__INCLUDED_)
