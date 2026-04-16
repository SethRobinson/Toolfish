#pragma once

#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

class CAction;

class CDlgActionAppVolume : public CBkDialogST
{
public:
    CDlgActionAppVolume(CWnd* pParent = NULL);
    void SetActionPointer(CAction *p_new) { m_p_action = p_new; }
    CAction *m_p_action;

    enum { IDD = IDD_ACTION_APP_VOLUME };

    int m_volume;
    BOOL m_b_relative;
    BOOL m_b_show_toast;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnRelativeClicked();
    DECLARE_MESSAGE_MAP()
};
