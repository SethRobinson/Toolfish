// CDlgActionSoundOutput.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionSoundOutput.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CAction.h"
#include "SoundOutputUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSoundOutput dialog

CDlgActionSoundOutput::CDlgActionSoundOutput(CWnd* pParent /*=NULL*/)
    : CBkDialogST(CDlgActionSoundOutput::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgActionSoundOutput)
    m_str_device_match = _T("");
    //}}AFX_DATA_INIT
}

void CDlgActionSoundOutput::DoDataExchange(CDataExchange* pDX)
{
    CBkDialogST::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgActionSoundOutput)
    DDX_Control(pDX, IDC_DEVICE_LIST, m_device_list);
    DDX_Text(pDX, IDC_DEVICE_MATCH, m_str_device_match);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgActionSoundOutput, CBkDialogST)
    //{{AFX_MSG_MAP(CDlgActionSoundOutput)
    ON_WM_CTLCOLOR()
    ON_LBN_SELCHANGE(IDC_DEVICE_LIST, OnDeviceListSelChange)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSoundOutput message handlers

void CDlgActionSoundOutput::PopulateDeviceList()
{
    m_device_list.ResetContent();
    
    std::vector<SoundOutputDevice> devices = EnumerateOutputDevices();
    
    for (size_t i = 0; i < devices.size(); i++)
    {
        CString deviceName(devices[i].friendlyName.c_str());
        m_device_list.AddString(deviceName);
    }
    
    if (devices.empty())
    {
        m_device_list.AddString(_T("(No active output devices found)"));
    }
}

HBRUSH CDlgActionSoundOutput::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CDlgActionSoundOutput::OnInitDialog()
{
    CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
    
    // Populate the device list
    PopulateDeviceList();
    
    // Load the saved device match string
    if (m_p_action->GetFilename() && m_p_action->GetFilename()[0] != L'\0')
    {
        m_str_device_match = m_p_action->GetFilename();
    }
    
    UpdateData(D_TO_WINDOW);
    return TRUE;
}

void CDlgActionSoundOutput::OnOK()
{
    UpdateData(D_TO_VAR);
    
    // Save the device match string using the filename field
    m_p_action->SetFilename(m_str_device_match);
    
    CBkDialogST::OnOK();
}

void CDlgActionSoundOutput::OnDeviceListSelChange()
{
    int sel = m_device_list.GetCurSel();
    if (sel != LB_ERR)
    {
        CString selectedText;
        m_device_list.GetText(sel, selectedText);
        
        // Don't copy the "no devices" message
        if (selectedText != _T("(No active output devices found)"))
        {
            // Update the edit box with the selected device name
            SetDlgItemText(IDC_DEVICE_MATCH, selectedText);
        }
    }
}
