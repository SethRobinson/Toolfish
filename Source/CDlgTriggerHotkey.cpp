// CDlgTriggerHotkey.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgTriggerHotkey.h"
#include "..\Shared\mfc_all.h"
#include "CTrigger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerHotkey dialog


CDlgTriggerHotkey::CDlgTriggerHotkey(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTriggerHotkey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTriggerHotkey)
	m_b_alt = FALSE;
	m_b_ctrl = FALSE;
	m_b_shift = FALSE;
	m_b_pass_through = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTriggerHotkey::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTriggerHotkey)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Check(pDX, IDC_B_ALT, m_b_alt);
	DDX_Check(pDX, IDC_B_CTRL, m_b_ctrl);
	DDX_Check(pDX, IDC_B_SHIFT, m_b_shift);
	DDX_Check(pDX, IDC_PASS_THROUGH, m_b_pass_through);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTriggerHotkey, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTriggerHotkey)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggerHotkey message handlers

void CDlgTriggerHotkey::OnOK() 
{
    //Return the correct data
    UpdateData(D_TO_VAR);

    m_p_action->m_b_alt = m_b_alt != 0;
    m_p_action->m_b_ctrl = m_b_ctrl != 0;
    m_p_action->m_b_shift = m_b_shift != 0;

    m_p_action->m_vk_key = char(m_combo.GetItemData(m_combo.GetCurSel()));
    
    m_p_action->m_b_pass_through = m_b_pass_through != 0;
	CBkDialogST::OnOK();
}

BOOL CDlgTriggerHotkey::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
    SetBitmap(_T("gui\\background.bmp"));
  
    PopulateCombo();
    
    m_b_alt = m_p_action->m_b_alt;
    m_b_shift = m_p_action->m_b_shift;
    m_b_ctrl = m_p_action->m_b_ctrl;
    m_b_pass_through = m_p_action->m_b_pass_through;
   
    // Cast to unsigned char to avoid sign extension issues with VK codes > 127
    set_selection_by_data(&m_combo, (unsigned char)m_p_action->m_vk_key);

    if (m_p_action->m_vk_key == 0)
    {
        //set a default
     set_selection_by_data(&m_combo, 'A');

  }

   UpdateData(D_TO_WINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTriggerHotkey::AddComboItem(TCHAR *p_name, int data)
{
    int i_index = m_combo.AddString(p_name);
    m_combo.SetItemData(i_index, data);

}

void CDlgTriggerHotkey::PopulateCombo()
{
    // Function keys F1-F24
    AddComboItem(_T("F1"), VK_F1);
    AddComboItem(_T("F2"), VK_F2);
    AddComboItem(_T("F3"), VK_F3);
    AddComboItem(_T("F4"), VK_F4);
    AddComboItem(_T("F5"), VK_F5);
    AddComboItem(_T("F6"), VK_F6);
    AddComboItem(_T("F7"), VK_F7);
    AddComboItem(_T("F8"), VK_F8);
    AddComboItem(_T("F9"), VK_F9);
    AddComboItem(_T("F10"), VK_F10);
    AddComboItem(_T("F11"), VK_F11);
    AddComboItem(_T("F12"), VK_F12);
    AddComboItem(_T("F13"), VK_F13);
    AddComboItem(_T("F14"), VK_F14);
    AddComboItem(_T("F15"), VK_F15);
    AddComboItem(_T("F16"), VK_F16);
    AddComboItem(_T("F17"), VK_F17);
    AddComboItem(_T("F18"), VK_F18);
    AddComboItem(_T("F19"), VK_F19);
    AddComboItem(_T("F20"), VK_F20);
    AddComboItem(_T("F21"), VK_F21);
    AddComboItem(_T("F22"), VK_F22);
    AddComboItem(_T("F23"), VK_F23);
    AddComboItem(_T("F24"), VK_F24);
    
    // Numbers
    AddComboItem(_T("1"), '1');
    AddComboItem(_T("2"), '2');
    AddComboItem(_T("3"), '3');
    AddComboItem(_T("4"), '4');
    AddComboItem(_T("5"), '5');
    AddComboItem(_T("6"), '6');
    AddComboItem(_T("7"), '7');
    AddComboItem(_T("8"), '8');
    AddComboItem(_T("9"), '9');
    AddComboItem(_T("0"), '0');

    // Letters
    AddComboItem(_T("A"), 'A');
    AddComboItem(_T("B"), 'B');
    AddComboItem(_T("C"), 'C');
    AddComboItem(_T("D"), 'D');
    AddComboItem(_T("E"), 'E');
    AddComboItem(_T("F"), 'F');
    AddComboItem(_T("G"), 'G');
    AddComboItem(_T("H"), 'H');
    AddComboItem(_T("I"), 'I');
    AddComboItem(_T("J"), 'J');
    AddComboItem(_T("K"), 'K');
    AddComboItem(_T("L"), 'L');
    AddComboItem(_T("M"), 'M');
    AddComboItem(_T("N"), 'N');
    AddComboItem(_T("O"), 'O');
    AddComboItem(_T("P"), 'P');
    AddComboItem(_T("Q"), 'Q');
    AddComboItem(_T("R"), 'R');
    AddComboItem(_T("S"), 'S');
    AddComboItem(_T("T"), 'T');
    AddComboItem(_T("U"), 'U');
    AddComboItem(_T("V"), 'V');
    AddComboItem(_T("W"), 'W');
    AddComboItem(_T("X"), 'X');
    AddComboItem(_T("Y"), 'Y');
    AddComboItem(_T("Z"), 'Z');

    // Punctuation
    AddComboItem(_T(","), ',');
    AddComboItem(_T("."), '.');
    AddComboItem(_T(";"), ';');
    AddComboItem(_T("'"), '\'');
    AddComboItem(_T("["), '[');
    AddComboItem(_T("]"), ']');
    AddComboItem(_T("-"), '-');
    AddComboItem(_T("="), '=');
    AddComboItem(_T("\\"), '\\');
    AddComboItem(_T("`"), '`');

    // Navigation keys
    AddComboItem(_T("Tab"), VK_TAB);
    AddComboItem(_T("Backspace"), VK_BACK);
    AddComboItem(_T("Delete"), VK_DELETE);
    AddComboItem(_T("Insert"), VK_INSERT);
    AddComboItem(_T("Home"), VK_HOME);
    AddComboItem(_T("End"), VK_END);
    AddComboItem(_T("Page Up"), VK_PRIOR);
    AddComboItem(_T("Page Down"), VK_NEXT);
    AddComboItem(_T("Arrow Left"), VK_LEFT);
    AddComboItem(_T("Arrow Up"), VK_UP);
    AddComboItem(_T("Arrow Right"), VK_RIGHT);
    AddComboItem(_T("Arrow Down"), VK_DOWN);

    // Media keys
    AddComboItem(_T("Volume Mute"), VK_VOLUME_MUTE);
    AddComboItem(_T("Volume Down"), VK_VOLUME_DOWN);
    AddComboItem(_T("Volume Up"), VK_VOLUME_UP);
    AddComboItem(_T("Media Next"), VK_MEDIA_NEXT_TRACK);
    AddComboItem(_T("Media Prev"), VK_MEDIA_PREV_TRACK);
    AddComboItem(_T("Media Stop"), VK_MEDIA_STOP);
    AddComboItem(_T("Media Play/Pause"), VK_MEDIA_PLAY_PAUSE);

    // System keys
    AddComboItem(_T("Print Screen"), VK_SNAPSHOT);
    AddComboItem(_T("Scroll Lock"), VK_SCROLL);
    AddComboItem(_T("Pause"), VK_PAUSE);
    AddComboItem(_T("Caps Lock"), VK_CAPITAL);
    AddComboItem(_T("Num Lock"), VK_NUMLOCK);

}

HBRUSH CDlgTriggerHotkey::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
	
 switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
        if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
              pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) GetStockObject(WHITE_BRUSH);
         break;
        }

        if (
            (pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
            )
        {
            // log_msg("Group box detected.");
            pDC->SetBkMode(TRANSPARENT);
            hbr=(HBRUSH) m_hbrush;
            return hbr;
        }
        
        //default 
        
        //pDC->SetTextColor(MY_TEXTCOLOR);
        pDC->SetBkMode(TRANSPARENT);
        hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
        return hbr;
        break;
      
    }
	return hbr;
}
