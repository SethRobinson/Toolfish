#if !defined(AFX_CDLGMUTE_H__C29ECC8F_F0B5_4E4B_9498_7C05147F0AF4__INCLUDED_)
#define AFX_CDLGMUTE_H__C29ECC8F_F0B5_4E4B_9498_7C05147F0AF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgMute.h : header file
//

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMute dialog
 void ActivateMute();
 void DeactivateMute();

class CDlgMute : public CBkDialogST
{
// Construction
public:
	CDlgMute(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMute)
	enum { IDD = IDD_MUTE };
	CSliderCtrl	m_slider;
	BOOL	m_b_smart_mute;
	CString	m_cst_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMute)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void CDlgMute::SetGreyOut();

	// Generated message map functions
	//{{AFX_MSG(CDlgMute)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancel();
	afx_msg void OnMuteEnabled();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGMUTE_H__C29ECC8F_F0B5_4E4B_9498_7C05147F0AF4__INCLUDED_)
