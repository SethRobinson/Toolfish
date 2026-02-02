#if !defined(AFX_CDLGTHREAD_H__304F7525_1A42_4993_B238_3907AAE092CB__INCLUDED_)
#define AFX_CDLGTHREAD_H__304F7525_1A42_4993_B238_3907AAE092CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgThread.h : header file
//
  #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgThread dialog

class CDlgThread : public CBkDialogST
{
// Construction
public:
	CDlgThread(CWnd* pParent = NULL);   // standard constructor
    int m_i_wait;
// Dialog Data
	//{{AFX_DATA(CDlgThread)
	enum { IDD = IDD_THREADS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgThread)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
   void CDlgThread::UpdateVisuals();
	// Generated message map functions
	//{{AFX_MSG(CDlgThread)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTHREAD_H__304F7525_1A42_4993_B238_3907AAE092CB__INCLUDED_)
