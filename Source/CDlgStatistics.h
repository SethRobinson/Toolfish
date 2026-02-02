#if !defined(AFX_CDLGSTATISTICS_H__3C5B8821_DDA0_45E5_AFB8_E786E6B91DA8__INCLUDED_)
#define AFX_CDLGSTATISTICS_H__3C5B8821_DDA0_45E5_AFB8_E786E6B91DA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgStatistics.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistics dialog

class CDlgStatistics : public CBkDialogST
{
// Construction
public:
	CDlgStatistics(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStatistics)
	enum { IDD = IDD_STATISTICS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStatistics)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    void CDlgStatistics::UpdateStats();

    // Generated message map functions
	//{{AFX_MSG(CDlgStatistics)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGSTATISTICS_H__3C5B8821_DDA0_45E5_AFB8_E786E6B91DA8__INCLUDED_)
