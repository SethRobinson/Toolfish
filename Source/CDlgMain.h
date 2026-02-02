// CDlgMain.h : header file
//

#if !defined(AFX_CDLGMAIN_H__01D1A7C6_B78B_4886_8B82_38BD5D9BD5E8__INCLUDED_)
#define AFX_CDLGMAIN_H__01D1A7C6_B78B_4886_8B82_38BD5D9BD5E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
#include "..\Shared\richeditctrlex.h"
#include "..\Shared\CTimer.h"
extern bool g_b_minimized;

/////////////////////////////////////////////////////////////////////////////
// CDlgMain dialog

class CDlgMain : public CBkDialogST
{
// Construction
public:
 
    void Log(CString cst_message);
    void AddToTray();
    void RemoveFromTray();
    void ShutDownStuff();
    void OpenDialogFromTray();
    void KillBitmaps();
    void FinalShutDown();
    void LoadButtons();
    void ProcessTray();

    
    CDlgMain(CWnd* pParent = NULL);	// standard constructor
    HICON m_hIcon_small;
    HICON m_hIcon_small_alarm;
    CTimer m_timer_tray;
    CTimer m_timer_maint;
    CTimer m_timer_auto_save;
    CTimer m_timer_ip_check;
      
    CButtonST m_but_events;
    // Dialog Data
	//{{AFX_DATA(CDlgMain)
	enum { IDD = IDD_SOURCE_DIALOG };
	CButtonST	m_but_fun;
	CButtonST	m_but_email;
	CButtonST	m_but_hide;
	CButtonST	m_but_clear;
	CButtonST	m_but_help;
	CButtonST	m_but_logo;
	CButtonST	m_but_options;
	CButtonST	m_but_suggestion;
	CButtonST	m_but_wizard;
	CButtonST	m_but_key_log;
	CButtonST	m_but_mute;
	CRichEditCtrlEx	m_cedit_log;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
 
    void OnPostEraseBkgnd(CDC* pDC);
    void TrayStartBlink(long i_code);
    void TrayStopBlink(long i_code);
    void SetupLog();
    
    int m_i_current_tray_icon;
    int m_i_current_tray_mode;
   
    HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CDlgMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEvents();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnKeyLog();
	afx_msg void OnOptions();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnOk();
	afx_msg void OnAbout();
	afx_msg void OnMute();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuOptions();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnWizard();
	afx_msg void OnClear();
	afx_msg void OnLogo();
	afx_msg void OnButHelp();
	afx_msg void OnMenuWizardWeb();
	afx_msg void OnMenuWizardOneTimeReminder();
	afx_msg void OnMenuVoiceMute();
	afx_msg void OnAboutToolfish();
	afx_msg void OnDisableSmartmute();
	afx_msg void OnMenuWizardReminder();
	afx_msg void OnMenuWizardAtomic();
	afx_msg void OnMenuReadClipboard();
	afx_msg void OnMenuWizardEmail();
	afx_msg void OnEmailview();
	afx_msg void OnMenuWizardMonitor();
	afx_msg void OnFun();
    afx_msg LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
	afx_msg void OnMenuWizardReminder1Day();
	afx_msg void OnMenuWizardReminder1Hour();
	afx_msg void OnMenuWizardReminder1Month();
	afx_msg void OnMenuWizardReminder1Week();
	afx_msg void OnMenuWizardReminder10();
	afx_msg void OnMenuWizardReminder15();
	afx_msg void OnMenuWizardReminder2Hour();
	afx_msg void OnMenuWizardReminder30();
	afx_msg void OnMenuWizardReminder4Hours();
	afx_msg void OnMenuWizardReminder5();
	afx_msg void OnMenuWizardReminder8Hours();
	afx_msg void OnMenuWizardReminder45();
	afx_msg void OnMenuWizardReminder20();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGMAIN_H__01D1A7C6_B78B_4886_8B82_38BD5D9BD5E8__INCLUDED_)
