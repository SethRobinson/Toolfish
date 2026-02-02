#if !defined(AFX_CDLGTRIGGERTIME_H__C1AABF53_BAD3_4C8A_8D60_EE66D7270954__INCLUDED_)
#define AFX_CDLGTRIGGERTIME_H__C1AABF53_BAD3_4C8A_8D60_EE66D7270954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTriggerTime.h : header file
//
class CTrigger; //forward for speed

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerTime dialog
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

class CDlgTriggerTime : public CBkDialogST
{
// Construction
public:
	CDlgTriggerTime(CWnd* pParent = NULL);   // standard constructor
     void CDlgTriggerTime::SetTriggerPointer(CTrigger *p_new);


// Dialog Data
	//{{AFX_DATA(CDlgTriggerTime)
	enum { IDD = IDD_TRIGGER_TIME };
	CMonthCalCtrl	m_date;
	CDateTimeCtrl	m_time;
	BOOL	m_b_reschedule_active;
	CString	m_cst_reschedule_time;
	BOOL	m_b_ignore_late;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTriggerTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CTrigger *m_p_trigger;
    void SetGreyOut();

    // Generated message map functions
	//{{AFX_MSG(CDlgTriggerTime)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRescheduleActive();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTRIGGERTIME_H__C1AABF53_BAD3_4C8A_8D60_EE66D7270954__INCLUDED_)
