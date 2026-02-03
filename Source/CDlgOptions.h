#if !defined(AFX_CDLGOPTIONS_H__66583E61_2F5B_45B3_ACFC_58628FF5B4A7__INCLUDED_)
#define AFX_CDLGOPTIONS_H__66583E61_2F5B_45B3_ACFC_58628FF5B4A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptions dialog
 #include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"


class CDlgOptions : public CBkDialogST
{
// Construction
public:
	CDlgOptions(CWnd* pParent = NULL);   // standard constructor
    void CDlgOptions::GrayOut();

// Dialog Data
	//{{AFX_DATA(CDlgOptions)
	enum { IDD = IDD_OPTIONS };
	BOOL	m_b_boot_load;
	int		m_i_log_lines;
	BOOL	m_b_trim_log;
	BOOL	m_b_disable_tts;
	BOOL	m_b_boot_stealth;
	BOOL	m_b_boot_admin;
	BOOL	m_b_enable_tray;
	BOOL	m_b_enable_blink_email;
	BOOL	m_b_enable_blink_webpage;
	BOOL	m_b_detect_IP_address_server_side;
	BOOL	m_b_minimize_on_close;
	BOOL	m_b_check_new_versions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void CDlgOptions::ProcessGray();
// Generated message map functions
	//{{AFX_MSG(CDlgOptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTrimLog();
	afx_msg void OnVoiceConfigure();
	afx_msg void OnBDisableTts();
	afx_msg void OnBootLoad();
	afx_msg void OnBootStealth();
	afx_msg void OnBootAdmin();
	afx_msg void OnSmtpConfigure();
	afx_msg void OnBEnableTray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGOPTIONS_H__66583E61_2F5B_45B3_ACFC_58628FF5B4A7__INCLUDED_)
