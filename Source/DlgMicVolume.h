#pragma once



#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
#include "afxcmn.h"
#include "afxwin.h"
class CAction;
// DlgMicVolume dialog

class DlgMicVolume: public CBkDialogST
{
	DECLARE_DYNAMIC(DlgMicVolume)

public:
	DlgMicVolume(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgMicVolume();

	void SetActionPointer(CAction *p_new) { m_p_action = p_new;}
	CAction *m_p_action;

// Dialog Data
	enum { IDD = IDD_ACTION_MIC_VOLUME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CSliderCtrl m_slider;
	afx_msg void OnEnChangePartialName();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString m_partial_name;
};
