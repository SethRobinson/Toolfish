// CDlgTest.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgTest.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"
#include "internet_util.h"
#include "spy_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTest dialog


CDlgTest::CDlgTest(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTest)
	m_cst_text = _T("");
	//}}AFX_DATA_INIT
}


void CDlgTest::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTest)
	DDX_Text(pDX, IDC_EDIT, m_cst_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTest, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTest)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_HTML, OnHtml)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTest message handlers

HBRUSH CDlgTest::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgTest::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();

    SetBitmap(_T("gui\\background.bmp"));
  
      
 
    m_cst_text.Format(_T("Please wait, loading web page %s..."), m_wst_url);
    
    
    
        if (!SetTimer(3, 1000, NULL ))
      {
          log_error("Unable to initalize final timer.");
      }

     
    
        UpdateData(D_TO_WINDOW);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDlgTest::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_TIMER)
    {
        if (wParam == 3)
        {
            KillTimer(3);
            //let's do it!
           GetPage();
           //unlock buttons?
        }
   }
	
	return CBkDialogST::WindowProc(message, wParam, lParam);
}


void CDlgTest::GetPage()
{
      UpdateData(D_TO_VAR);

 
    TCHAR ws_domain[C_MAX_URL_SIZE];
    TCHAR ws_path[C_MAX_URL_SIZE];
    TCHAR ws_filename[C_MAX_URL_SIZE];

    TCHAR *p_full = (TCHAR*)&m_wst_url;
  
    // *****************

    TCHAR *p_temp_parms = NULL;
   
    //get rid of the starting http if applicapable
    if (!BreakURLDown((TCHAR*)&m_wst_url, p_full, ws_domain, ws_path, ws_filename))
    {
        LogError(_T("Error understanding URL, maybe you typed it wrong? (%s), "), m_wst_url);
        
    }  else
    {
        
        CTextParse new_content;
        CWebHeader new_header;
        
        if (GetWebFile(uni(ws_domain).to_st(), uni(ws_path).to_st(), &new_content, &new_header, m_b_authenticate, uni(m_cst_logon).us_data, uni(m_cst_pass).us_data))
        {
            
            m_cst_text = new_content.GetBuffer();
            
        } else
        {
            m_cst_text += "Could not get webpage.  Link must be bad or down.";
            
        }
    }

       UpdateData(D_TO_WINDOW);


}



void CDlgTest::OnHtml() 
{
     UpdateData(D_TO_VAR);

    //let's save it out to disk...
    TCHAR * p_filename = _T("temp.htm");
    _tunlink(p_filename);
    int i_index;
    if (-1 != (i_index = m_cst_text.Find(_T("Content-Type: text/html\r\n\r\n"))))
   {
       //found it!
        m_cst_text.Delete(0, i_index+_tcslen(_T("Content-Type: text/html\r\n\r\n")));
      
   }
    AddText(m_cst_text, p_filename);  
    int result = (int)ShellExecute(NULL,_T("open"),p_filename, NULL,NULL, SW_MAXIMIZE);
    ProcessError(this->m_hWnd, result, _T("temp.url"),p_filename ); //show an error message if we couldn't open this
     UpdateData(D_TO_VAR);
  
}
