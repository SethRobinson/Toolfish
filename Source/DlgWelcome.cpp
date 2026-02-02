// DlgWelcome.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "DlgWelcome.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"
#include "Mmsystem.h."

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWelcome dialog


CDlgWelcome::CDlgWelcome(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgWelcome::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWelcome)
	//}}AFX_DATA_INIT
}


void CDlgWelcome::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWelcome)
	DDX_Control(pDX, IDC_BUT_TIP4, m_but_tip4);
	DDX_Control(pDX, IDC_BUT_TIP3, m_but_tip3);
	DDX_Control(pDX, IDC_BUT_TIP2, m_but_tip2);
	DDX_Control(pDX, IDC_BUT_TIP1, m_but_tip1);
	DDX_Control(pDX, IDC_PIC, m_but_pic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWelcome, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgWelcome)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWelcome message handlers

BOOL CDlgWelcome::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
	// Ensure dialog is shown in foreground and not minimized on first launch
	ShowWindow(SW_RESTORE);
	SetForegroundWindow();
	BringWindowToTop();

  SetBitmap(_T("gui\\background.bmp"));
	 m_but_pic.SetTooltipText(_T("Welcome to Toolfish!  Why is called that?  Hmm, good question."));
   m_but_pic.DrawTransparent();
   m_but_pic.SetBitmaps(_T("gui\\but_event.bmp"), RGB(255,255,255), 0,0);
   //m_but_pic.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
  // m_but_pic.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
   m_but_pic.SizeToContent();
   m_but_pic.DrawBorder(false, false);

   
  m_but_tip1.DrawTransparent();
  m_but_tip1.SetIcon(IDR_MAINFRAME);
  m_but_tip1.SizeToContent();
  m_but_tip1.DrawBorder(false, false);
   
 m_but_tip2.DrawTransparent();
  m_but_tip2.SetIcon(IDR_MAINFRAME);
  m_but_tip2.SizeToContent();
  m_but_tip2.DrawBorder(false, false);

   m_but_tip3.DrawTransparent();
  m_but_tip3.SetIcon(IDR_MAINFRAME);
  m_but_tip3.SizeToContent();
  m_but_tip3.DrawBorder(false, false);
 
  m_but_tip4.DrawTransparent();
  m_but_tip4.SetIcon(IDR_MAINFRAME);
  m_but_tip4.SizeToContent();
  m_but_tip4.DrawBorder(false, false);



 	     PlaySound(_T("media\\secret.wav"), NULL, SND_FILENAME | SND_ASYNC);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDlgWelcome::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);
	
switch (nCtlColor)
    {
        
        // TODO: Change any attributes of the DC here
      case CTLCOLOR_STATIC:  
 
        if (pWnd->GetDlgCtrlID() == IDC_LOG)
        {
          //    pDC->SetBkMode(TRANSPARENT);
            pDC->SetBkColor(RGB(255,255,255));
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
        hbr=(HBRUSH) (m_hbrush);
        return hbr;
        break;
      
    }
	

	return hbr;
}
