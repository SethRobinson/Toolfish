#if !defined(AFX_CDLGACTIONTTS_H__8380A89D_04ED_4883_965E_4858387935A0__INCLUDED_)
#define AFX_CDLGACTIONTTS_H__8380A89D_04ED_4883_965E_4858387935A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionTTS.h : header file
//
 class CAction;  //forward declaration for compiling speed
  
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgActionTTS dialog

class CDlgActionTTS : public CBkDialogST
{
// Construction
public:
	CDlgActionTTS(CWnd* pParent = NULL);   // standard constructor
        void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
    CAction *m_p_action;

// Dialog Data
	//{{AFX_DATA(CDlgActionTTS)
	enum { IDD = IDD_ACTION_TTS };
	CComboBox	m_combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgActionTTS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgActionTTS)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONTTS_H__8380A89D_04ED_4883_965E_4858387935A0__INCLUDED_)
