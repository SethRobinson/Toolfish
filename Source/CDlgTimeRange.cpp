// CDlgTimeRange.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgTimeRange.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "CGlobals.h"
#include "CTrigger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeRange dialog


CDlgTimeRange::CDlgTimeRange(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgTimeRange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTimeRange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgTimeRange::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTimeRange)
	DDX_Control(pDX, IDC_TIME_END, m_time_end);
	DDX_Control(pDX, IDC_TIME_START, m_time_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTimeRange, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgTimeRange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgTimeRange::SetTriggerPointer(CTrigger *p_new)
{
    m_p_trigger = p_new;
}


/////////////////////////////////////////////////////////////////////////////
// CDlgTimeRange message handlers

HBRUSH CDlgTimeRange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgTimeRange::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
 SetBitmap(_T("gui\\background.bmp"));
    m_time_start.SetTime(&m_p_trigger->GetTime());	
    m_time_end.SetTime(&m_p_trigger->GetTimeEnd());	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTimeRange::OnOK() 
{
    //add it back
    CTime temp;
    m_time_start.GetTime(temp);
    m_p_trigger->SetTime(&temp);    

      m_time_end.GetTime(temp);
    m_p_trigger->SetTimeEnd(&temp);    



	CBkDialogST::OnOK();
}
