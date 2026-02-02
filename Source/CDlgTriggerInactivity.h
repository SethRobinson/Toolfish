#if !defined(AFX_CDLGTRIGGERINACTIVITY_H__4F3D3DD7_3F2F_4AC3_8C13_4B29EA1196D9__INCLUDED_)
#define AFX_CDLGTRIGGERINACTIVITY_H__4F3D3DD7_3F2F_4AC3_8C13_4B29EA1196D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTriggerInactivity.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

class CTrigger; //forward for speed

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerInactivity dialog

class CDlgTriggerInactivity : public CBkDialogST
{
// Construction
public:
	CDlgTriggerInactivity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTriggerInactivity)
	enum { IDD = IDD_TRIGGER_INACTIVITY };
	CString	m_cst_edit;
	//}}AFX_DATA

    void SetTriggerPointer(CTrigger *p_new){m_p_trigger = p_new;}
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTriggerInactivity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CTrigger *m_p_trigger;
  

	// Generated message map functions
	//{{AFX_MSG(CDlgTriggerInactivity)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTRIGGERINACTIVITY_H__4F3D3DD7_3F2F_4AC3_8C13_4B29EA1196D9__INCLUDED_)
