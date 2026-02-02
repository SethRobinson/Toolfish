// CDlgAbout.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgAbout.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CDlgUnlock.h"
#include "CGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog


CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAbout)
	//}}AFX_DATA_INIT
}


void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbout)
	DDX_Control(pDX, IDC_TOOLFISH_HELP, m_but_toolfish_help);
	DDX_Control(pDX, IDC_RTSOF, m_but_rtsoft_logo);
	DDX_Control(pDX, IDC_LOGO, m_but_logo);
	DDX_Control(pDX, IDC_BUT_RTSOFT, m_but_rtsoft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAbout, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgAbout)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TOOLFISH_HELP, OnToolfishHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAbout message handlers

HBRUSH CDlgAbout::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgAbout::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
  SetBitmap(_T("gui\\background.bmp"));
           CString cst_temp;
#ifdef _UNICODE
           WCHAR wc_char = app_glo.GetBuild();
           cst_temp.Format(_T("%s V%.2f%c by Seth & Akiko Robinson."), app_glo.GetAppName(), app_glo.GetVersion(), wc_char);

#else
           cst_temp.Format(_T("%s V%.2f%c by Seth & Akiko Robinson."), app_glo.GetAppName(), app_glo.GetVersion(),  app_glo.GetBuild());
#endif         
         ((CStatic*)GetDlgItem(IDC_ABOUT_STATIC))->SetWindowText(cst_temp);
 	
	// TODO: Add extra initialization here
 m_but_rtsoft.SetTooltipText(_T("Visit the Robinson Technologies website.  It's a cool site, do it!"));
   m_but_rtsoft.DrawTransparent();
 //  m_but_rtsoft.SetBitmaps(_T("gui\\but_event.bmp"), RGB(255,255,255), 0,0);
   m_but_rtsoft.SetAlign(CButtonST::ST_ALIGN_HORIZ_RIGHT, FALSE);
  // m_but_events.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
   // m_but_events.SizeToContent();
   m_but_rtsoft.DrawBorder(false, false);
   m_but_rtsoft.SetURL(_T("http://www.rtsoft.com"));
   m_but_rtsoft.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(20,20,255), false);
   m_but_rtsoft.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(255,0,0), false);

   
     m_but_logo.DrawTransparent();
     m_but_logo.SetBitmaps(_T("gui\\logo_medium.bmp"), RGB(255,255,255), 0,0);
     //m_but_logo.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
     // m_but_logo.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
     m_but_logo.SizeToContent();
     m_but_logo.DrawBorder(false, false);
     m_but_logo.SetURL(_T("http://www.rtsoft.com/toolfish"))	;
     m_but_logo.SetTooltipText(_T("Click here to visit the Toolfish website."));

     m_but_rtsoft_logo.DrawTransparent();
     m_but_rtsoft_logo.SetBitmaps(_T("gui\\rtsoft_logo.bmp"), RGB(0,0,0), 0,0);
     //m_but_rtsoft_logo.SetAlign(CButtonST::ST_ALIGN_VERT, FALSE);
     // m_but_rtsoft_logo.SetPressedStyle(CButtonST::BTNST_PRESSED_TOPBOTTOM, FALSE);
     m_but_rtsoft_logo.SizeToContent();
     m_but_rtsoft_logo.DrawBorder(false, false);
     m_but_rtsoft_logo.SetURL(_T("http://www.rtsoft.com"))	;
     m_but_rtsoft_logo.SetTooltipText(_T("Click here to visit the Robinson Technologies website."));
  
     
     
   m_but_toolfish_help.SetTooltipText(_T("Get the most of Toolfish by reading the help.  Some stuff is tricky."));
   m_but_toolfish_help.DrawTransparent();
   m_but_toolfish_help.SetBitmaps(_T("gui\\icon_help.bmp"), RGB(255,255,255), 0,0);
   m_but_toolfish_help.SetAlign(CButtonST::ST_ALIGN_HORIZ, FALSE);
 
     
     return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAbout::OnToolfishHelp() 
{
	// TODO: Add your control notification handler code here
 int result = (int)ShellExecute(NULL,_T("open"),_T("docs\\help.htm"), NULL,NULL, SW_MAXIMIZE);
  ProcessError(this->m_hWnd, result, _T("temp.url"),_T("docs\\help.htm")); //show an error message if we couldn't open this
	
}
