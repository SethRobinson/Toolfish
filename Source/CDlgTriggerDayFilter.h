#if !defined(AFX_CDLGTRIGGERDAYFILTER_H__2A4C9772_B08F_471B_8A0F_2D50A508823B__INCLUDED_)
#define AFX_CDLGTRIGGERDAYFILTER_H__2A4C9772_B08F_471B_8A0F_2D50A508823B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTriggerDayFilter.h : header file
//
class CTrigger; //forward for speed
/////////////////////////////////////////////////////////////////////////////
// CDlgTrigger dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"



/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerDayFilter dialog

class CDlgTriggerDayFilter : public CBkDialogST
{
// Construction
public:
	CDlgTriggerDayFilter(CWnd* pParent = NULL);   // standard constructor
     CTrigger * m_p_trigger;
    void SetTriggerPointer(CTrigger *p_new) { m_p_trigger = p_new;}


// Dialog Data
	//{{AFX_DATA(CDlgTriggerDayFilter)
	enum { IDD = IDD_TRIGGER_DAY_FILTER };
	BOOL	m_b_0;
	BOOL	m_b_1;
	BOOL	m_b_2;
	BOOL	m_b_3;
	BOOL	m_b_4;
	BOOL	m_b_5;
	BOOL	m_b_6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTriggerDayFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTriggerDayFilter)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTRIGGERDAYFILTER_H__2A4C9772_B08F_471B_8A0F_2D50A508823B__INCLUDED_)
