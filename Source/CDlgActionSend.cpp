// CDlgActionSend.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgActionSend.h"
 #include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CDlgAlert.h"
#include "ProcessFindUtils.h"

#include "CAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSend dialog


CDlgActionSend::CDlgActionSend(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgActionSend::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionSend)
	m_cst_search = _T("");
	m_b_alt = FALSE;
	m_b_ctrl = FALSE;
	m_b_shift = FALSE;
	m_dw_lparam = 0;
	m_dw_wparam = 0;
	m_ui_msg = 0;
	//}}AFX_DATA_INIT
}


void CDlgActionSend::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionSend)
	DDX_Control(pDX, IDC_VIEW, m_but_view);
	DDX_Control(pDX, IDC_COMBO_KEY, m_combo_key);
	DDX_Control(pDX, IDC_COMBO_MESSAGE, m_combo_message);
	DDX_Control(pDX, IDC_FILENAME, m_combo_filename);
	DDX_Text(pDX, IDC_SEARCH, m_cst_search);
	DDX_Check(pDX, IDC_B_ALT, m_b_alt);
	DDX_Check(pDX, IDC_B_CTRL, m_b_ctrl);
	DDX_Check(pDX, IDC_B_SHIFT, m_b_shift);
	DDX_Text(pDX, IDC_LPARAM, m_dw_lparam);
	DDX_Text(pDX, IDC_WPARAM, m_dw_wparam);
	DDX_Text(pDX, IDC_MSG, m_ui_msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionSend, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgActionSend)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_VIEW, OnView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionSend message handlers

HBRUSH CDlgActionSend::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
            || (pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
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

int CDlgActionSend::AddMessage(TCHAR * p_t_text, int i_data_code)
{
    int i_temp = m_combo_message.AddString(p_t_text);
    m_combo_message.SetItemData(i_temp, i_data_code);
   return i_temp; //just in case they wanted to know what index it was.. 
}


void CDlgActionSend::AddComboItem(TCHAR *p_name, int data)
{
    int i_index = m_combo_key.AddString(p_name);
    m_combo_key.SetItemData(i_index, data);

}


BOOL CDlgActionSend::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	SetBitmap(_T("gui\\background.bmp"));

         m_but_view.DrawBorder(false, false);
  // m_but_view.SetURL(L"http://www.view.com")	;
   m_but_view.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(20,20,255), false);
   m_but_view.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0), false);
    m_but_view.DrawTransparent();


    m_combo_filename.ResetContent();
    m_combo_filename.AddString(_T("in a process .exe name (exact match)"));
    m_combo_filename.AddString(_T("in a process window title text (substring ok)"));
  
   m_combo_message.ResetContent();
    AddMessage(_T("Close"), C_ACTION_WM_QUIT);
    AddMessage(_T("Minimize"), C_ACTION_WM_MINIMIZE);
    AddMessage(_T("Maximize"), C_ACTION_WM_MAXIMIZE);
    AddMessage(_T("Set Focus To"), C_ACTION_WM_FOCUS);
    AddMessage(_T("Close and kill-task if required"), C_ACTION_WM_QUIT_HARD);
    AddMessage(_T("Fake click to OK button"), C_ACTION_WM_OK);
    AddMessage(_T("Fake click to CANCEL button"), C_ACTION_WM_CANCEL);
	AddMessage(_T("Mouse left mouse button"), C_ACTION_WM_MOUSE_LEFT_CLICK);
    
  
   m_combo_key.ResetContent(); //clear it out before filling it with this stuff 

    // Special keys (legacy names kept for compatibility)
    AddComboItem(_T("_ENTER_"), VK_RETURN);
    AddComboItem(_T("_ESCAPE_"), VK_ESCAPE);
    AddComboItem(_T("_SPACE_"), VK_SPACE);
    
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
 
    
    m_combo_filename.SetCurSel(m_p_action->GetProcessSearchField());
 
    m_cst_search = m_p_action->GetBufferPointer();

    set_selection_by_data(&m_combo_message, m_p_action->GetProcessWMCommand());
  

      //set radio buttons
    CButton* p_button;
    if (m_p_action->GetProcessMessageType() == C_ACTION_MESSAGE_NORMAL)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO1);  
    }
    if (m_p_action->GetProcessMessageType() == C_ACTION_MESSAGE_KEY)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO2);  
    }
    if (m_p_action->GetProcessMessageType() == C_ACTION_MESSAGE_CUSTOM)
    {
      p_button = (CButton*)GetDlgItem(IDC_RADIO3);  
    }
  
    p_button->SetCheck(1);


    //set the keypress info
    // Cast to unsigned char to avoid sign extension issues with VK codes > 127
    set_selection_by_data(&m_combo_key, (unsigned char)m_p_action->GetVK());
    m_b_shift = m_p_action->GetShift();
    m_b_ctrl = m_p_action->GetCtrl();
    m_b_alt = m_p_action->GetAlt();
  
    m_ui_msg = m_p_action->GetCustomMsg();
    m_dw_lparam = m_p_action->GetCustomLParam();
    m_dw_wparam = m_p_action->GetCustomWParam();


    UpdateData(D_TO_WINDOW);
	
     GrayOutStuff();



    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgActionSend::GrayOutStuff()
{
   bool b_message =  ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck()!=0;
   ((CButton*)GetDlgItem(IDC_COMBO_MESSAGE))->EnableWindow(b_message);


   bool b_key =  ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck()!=0;
   ((CButton*)GetDlgItem(IDC_COMBO_KEY))->EnableWindow(b_key);
   ((CButton*)GetDlgItem(IDC_B_SHIFT))->EnableWindow(b_key);
   ((CButton*)GetDlgItem(IDC_B_CTRL))->EnableWindow(b_key);
   ((CButton*)GetDlgItem(IDC_B_ALT))->EnableWindow(b_key);



    bool b_custom =  ((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck()!=0;
   ((CButton*)GetDlgItem(IDC_MSG))->EnableWindow(b_custom);
   ((CButton*)GetDlgItem(IDC_LPARAM))->EnableWindow(b_custom);
   ((CButton*)GetDlgItem(IDC_WPARAM))->EnableWindow(b_custom);
 
   InvalidateRect(NULL);

}


void CDlgActionSend::OnOK() 
{
	UpdateData(D_TO_VAR);
   	
    m_p_action->SetProcessSearchField(m_combo_filename.GetCurSel());
    m_p_action->CopyToBuffer(uni(m_cst_search).us_data);
    m_p_action->SetProcessWMCommand(m_combo_message.GetItemData(m_combo_message.GetCurSel()));

    
     if ( ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
    {
        m_p_action->SetProcessMessageType(C_ACTION_MESSAGE_NORMAL);
    }
     if ( ((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
    {
        m_p_action->SetProcessMessageType(C_ACTION_MESSAGE_KEY);
    }
     if ( ((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())
    {
        m_p_action->SetProcessMessageType(C_ACTION_MESSAGE_CUSTOM);
    }                                                             
   
    //set keystroke info
     m_p_action->SetVK(char(m_combo_key.GetItemData(m_combo_key.GetCurSel())));
     m_p_action->SetShift(m_b_shift != 0);
     m_p_action->SetCtrl(m_b_ctrl != 0);
     m_p_action->SetAlt(m_b_alt != 0);

     m_p_action->SetCustomMsg(m_ui_msg);
     m_p_action->SetCustomWParam(m_dw_wparam);
     m_p_action->SetCustomLParam(m_dw_lparam);


   
    CBkDialogST::OnOK();
}



void CDlgActionSend::OnRadio1() 
{
    GrayOutStuff();	
}

void CDlgActionSend::OnRadio2() 
{
    GrayOutStuff();	
}

void CDlgActionSend::OnRadio3() 
{
    GrayOutStuff();	
}

void CDlgActionSend::OnView() 
{
 CDlgAlert dlg;

    dlg.SetWindowCaption(_T("Active process information"));
    CString cst_text;

    BuildListOfActiveProcesses(cst_text);

    dlg.StoreTextPointerForLater(cst_text);

    dlg.DoModal();

	
}
