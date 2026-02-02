// CDlgThread.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgThread.h"
#include "CGLobals.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgThread dialog


CDlgThread::CDlgThread(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgThread::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgThread)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgThread::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgThread)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgThread, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgThread)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgThread message handlers

BOOL CDlgThread::OnInitDialog() 
{
    m_i_wait = 0; //we haven't waited at all so far..

    CBkDialogST::OnInitDialog();
	app_glo.SetPaused(true); //don't allow new events to run, we're shutting down
	  SetBitmap(_T("gui\\background.bmp"));
      if (!SetTimer(2, 1000, NULL ))
      {
          log_error("Unable to initalize final timer.");
      }
      
	  UpdateVisuals();

     //let's get our own private timer going
      
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgThread::UpdateVisuals()
{
   CString st_temp;
   st_temp.Format(_T("There are %d events currently being run, please wait as they finish up..."),
       app_glo.GetActiveThreadCount());
      
   for (int i=0; i < m_i_wait; i++)
   {
       st_temp += L"."; //add some dot animation 
   }

   if (m_i_wait < 5)
   {
   ((CButton*)GetDlgItem(IDOK))->EnableWindow(false);
   } else
   {
       ((CButton*)GetDlgItem(IDOK))->EnableWindow(true);
   }

     ((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(st_temp);

}

void CDlgThread::OnOK() 
{
	// TODO: Add extra validation here
	
	CBkDialogST::OnOK();
}

HBRUSH CDlgThread::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

LRESULT CDlgThread::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_TIMER)
    {
        if (wParam == 2)
        {
            m_i_wait++;
            UpdateVisuals();
            if (app_glo.GetActiveThreadCount() < 1)
            {
                //app_glo.SetPaused(true); //don't allow new events to run, we're shutting down
                OnOK(); //all done, finally
            }
        }
   }
	return CBkDialogST::WindowProc(message, wParam, lParam);
}
