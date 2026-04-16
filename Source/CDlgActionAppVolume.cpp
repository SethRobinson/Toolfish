#include "stdafx.h"
#include "source.h"
#include "CDlgActionAppVolume.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgActionAppVolume::CDlgActionAppVolume(CWnd* pParent)
    : CBkDialogST(CDlgActionAppVolume::IDD, pParent)
{
    m_volume = 50;
    m_b_relative = FALSE;
    m_b_show_toast = TRUE;
}

void CDlgActionAppVolume::DoDataExchange(CDataExchange* pDX)
{
    CBkDialogST::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_APP_VOL_EDIT, m_volume);
    DDX_Check(pDX, IDC_APP_VOL_RELATIVE, m_b_relative);
    DDX_Check(pDX, IDC_APP_VOL_SHOW_TOAST, m_b_show_toast);
}

BEGIN_MESSAGE_MAP(CDlgActionAppVolume, CBkDialogST)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_APP_VOL_RELATIVE, OnRelativeClicked)
END_MESSAGE_MAP()

HBRUSH CDlgActionAppVolume::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);

    switch (nCtlColor)
    {
    case CTLCOLOR_STATIC:
        if (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
        {
            pDC->SetBkMode(TRANSPARENT);
            hbr = (HBRUSH)m_hbrush;
            return hbr;
        }
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
        return hbr;
    }

    return hbr;
}

BOOL CDlgActionAppVolume::OnInitDialog()
{
    CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));

    m_volume = m_p_action->GetVolume();
    m_b_relative = m_p_action->GetFade() ? TRUE : FALSE;
    m_b_show_toast = m_p_action->GetTTS() ? TRUE : FALSE;

    if (m_b_relative)
    {
        CEdit* pEdit = (CEdit*)GetDlgItem(IDC_APP_VOL_EDIT);
        if (pEdit)
            pEdit->ModifyStyle(ES_NUMBER, 0);
    }

    UpdateData(D_TO_WINDOW);
    return TRUE;
}

void CDlgActionAppVolume::OnOK()
{
    UpdateData(D_TO_VAR);

    if (m_b_relative)
    {
        if (m_volume < -100) m_volume = -100;
        if (m_volume > 100) m_volume = 100;
    }
    else
    {
        if (m_volume < 0) m_volume = 0;
        if (m_volume > 100) m_volume = 100;
    }

    m_p_action->SetVolume(m_volume);
    m_p_action->SetFade(m_b_relative != 0);
    m_p_action->SetTTS(m_b_show_toast != 0);

    CBkDialogST::OnOK();
}

void CDlgActionAppVolume::OnRelativeClicked()
{
    UpdateData(D_TO_VAR);

    // When switching to relative, allow negative values by toggling ES_NUMBER
    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_APP_VOL_EDIT);
    if (pEdit)
    {
        if (m_b_relative)
            pEdit->ModifyStyle(ES_NUMBER, 0);
        else
            pEdit->ModifyStyle(0, ES_NUMBER);
    }
}
