#if !defined(AFX_CDLGVOICE_H__D8575192_F66C_403E_BD74_B4E79E8CC0CD__INCLUDED_)
#define AFX_CDLGVOICE_H__D8575192_F66C_403E_BD74_B4E79E8CC0CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgVoice.h : header file
//

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

class CVoice;

/////////////////////////////////////////////////////////////////////////////
// CDlgVoice dialog

class CDlgVoice : public CBkDialogST
{
// Construction
public:
	CDlgVoice(CWnd* pParent = NULL);   // standard constructor
    void SetPointerToVoiceClass(CVoice *p_voice){m_p_voice = p_voice;}

// Dialog Data
	//{{AFX_DATA(CDlgVoice)
	enum { IDD = IDD_VOICE };
	CSliderCtrl	m_slider_speed;
	CSliderCtrl	m_slider_pitch;
	CString	m_cst_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVoice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CVoice *m_p_voice;
	// Generated message map functions
	//{{AFX_MSG(CDlgVoice)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHear();
	afx_msg void OnSpeechProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGVOICE_H__D8575192_F66C_403E_BD74_B4E79E8CC0CD__INCLUDED_)
