#if !defined(AFX_CDLGTIMERANGE_H__0E153256_9BEB_414E_8F07_B99EC253C1A7__INCLUDED_)
#define AFX_CDLGTIMERANGE_H__0E153256_9BEB_414E_8F07_B99EC253C1A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgTimeRange.h : header file
//

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgTimeRange dialog
class CTrigger; //forward for speed


class CDlgTimeRange : public CBkDialogST
{
// Construction
public:
	CDlgTimeRange(CWnd* pParent = NULL);   // standard constructor
     void SetTriggerPointer(CTrigger *p_new);
     CTrigger *m_p_trigger;
  

// Dialog Data
	//{{AFX_DATA(CDlgTimeRange)
	enum { IDD = IDD_TIME_RANGE };
	CDateTimeCtrl	m_time_end;
	CDateTimeCtrl	m_time_start;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTimeRange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTimeRange)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGTIMERANGE_H__0E153256_9BEB_414E_8F07_B99EC253C1A7__INCLUDED_)
