#if !defined(AFX_CDLGACTIONDELAY_H__CF3A1B61_A661_4155_A98F_DD8DCC081495__INCLUDED_)
#define AFX_CDLGACTIONDELAY_H__CF3A1B61_A661_4155_A98F_DD8DCC081495__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionDelay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgActionDelay dialog
class CAction;  //forward declaration for compiling speed

 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgActionDelay : public CBkDialogST
{
// Construction
public:
	CDlgActionDelay(CWnd* pParent = NULL);   // standard constructor
   void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionDelay)
	enum { IDD = IDD_ACTION_DELAY };
	int		m_i_ms;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionDelay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionDelay)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONDELAY_H__CF3A1B61_A661_4155_A98F_DD8DCC081495__INCLUDED_)
