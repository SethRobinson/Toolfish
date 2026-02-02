#if !defined(AFX_CDLGACTIONHOTKEY_H__0EE1228F_CD72_4E33_A81B_F086922689F0__INCLUDED_)
#define AFX_CDLGACTIONHOTKEY_H__0EE1228F_CD72_4E33_A81B_F086922689F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionHotKey.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgActionHotKey dialog

class CDlgActionHotKey : public CDialog
{
// Construction
public:
	CDlgActionHotKey(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgActionHotKey)
	enum { IDD = IDD_ACTION_HOTKEY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionHotKey)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionHotKey)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONHOTKEY_H__0EE1228F_CD72_4E33_A81B_F086922689F0__INCLUDED_)
