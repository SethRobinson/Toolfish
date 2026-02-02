#if !defined(AFX_CDLGACTIONSMARTMUTE_H__EA5C24F2_9D72_4B90_89A5_453A517AD030__INCLUDED_)
#define AFX_CDLGACTIONSMARTMUTE_H__EA5C24F2_9D72_4B90_89A5_453A517AD030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionSmartMute.h : header file
//
 
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionSmartMute dialog
class CAction;
class CDlgActionSmartMute : public CBkDialogST
{
// Construction
public:
	CDlgActionSmartMute(CWnd* pParent = NULL);   // standard constructor
     void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;


// Dialog Data
	//{{AFX_DATA(CDlgActionSmartMute)
	enum { IDD = IDD_ACTION_SMARTMUTE };
	CComboBox	m_combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionSmartMute)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionSmartMute)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONSMARTMUTE_H__EA5C24F2_9D72_4B90_89A5_453A517AD030__INCLUDED_)
