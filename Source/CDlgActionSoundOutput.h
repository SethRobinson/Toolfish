#if !defined(AFX_CDLGACTIONSOUNDOUTPUT_H__INCLUDED_)
#define AFX_CDLGACTIONSOUNDOUTPUT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgActionSoundOutput.h : header file
//

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSoundOutput dialog
class CAction;

class CDlgActionSoundOutput : public CBkDialogST
{
// Construction
public:
    CDlgActionSoundOutput(CWnd* pParent = NULL);   // standard constructor
    void SetActionPointer(CAction *p_new) { m_p_action = p_new; }
    CAction *m_p_action;

// Dialog Data
    //{{AFX_DATA(CDlgActionSoundOutput)
    enum { IDD = IDD_ACTION_SOUND_OUTPUT };
    CListBox m_device_list;
    CString m_str_device_match;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgActionSoundOutput)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    void PopulateDeviceList();

    // Generated message map functions
    //{{AFX_MSG(CDlgActionSoundOutput)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnDeviceListSelChange();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGACTIONSOUNDOUTPUT_H__INCLUDED_)
