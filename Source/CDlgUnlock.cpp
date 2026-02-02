// CDlgUnlock.cpp : implementation file
//

#include "stdafx.h"
#include "source.h"
#include "CDlgUnlock.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"
#include "security_online.h"
#include "file_config.h"
#include "Mmsystem.h."


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUnlock dialog

CDlgUnlock::CDlgUnlock(CWnd* pParent /*=NULL*/)
	: CBkDialogST(CDlgUnlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUnlock)
	m_ws_key = _T("");
	//}}AFX_DATA_INIT
}


void CDlgUnlock::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUnlock)
	DDX_Text(pDX, IDC_KEY, m_ws_key);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUnlock, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgUnlock)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUnlock message handlers

HBRUSH CDlgUnlock::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CDlgUnlock::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	
  SetBitmap(_T("gui\\background.bmp"));
  	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUnlock::OnOK() 
{
	//get the real code from RTsoft
   //start check procedure
               SetDlgItemText(IDC_STATUS, _T("Finding RTsoft server..."));
                
               if (!server_info.GetServerInfo("www.rtsoft.com", "toolfish/server.txt"))
                {
                    //unable to connect, let them play anyway.
                          
                   int i_ret = MessageBox(_T("Unable to connect with www.rtsoft.com.\n\nIf you are not currently connected to the internet you will need to connect to unlock the game.\n\nIt also may be down or rebooting, please try again later.  Sorry for the hassle!"), _T("Error connecting"), MB_OK);
                    OnCancel();
                    return;
                }
                      SetDlgItemText(IDC_STATUS, _T("Found server.  Contacting RTsoft, please wait..."));
              
                //get input
                UpdateData(D_TO_VAR);		

                //go ahead and communicate with the main server
                char st_message[256];
                char st_key[100];
                if (!server_info.GetAuthorization(uni(m_ws_key).to_st(), st_key, st_message, false))
                {
                  	//write new settings
                    glo.m_st_code[0]=0;
                    glo.m_st_unlock_key[0]=0;

                    //g_settings.ResetKey();
                    
#ifdef _UNICODE
                    MessageBox(uni(st_message).us_data, _T("Error"), NULL);
#else
                    MessageBox(st_message, _T("Error"), NULL);
                    
#endif
                    
                    OnCancel();
                    return;
                }
                                
           //let's copy whatever it is into our global area
             //g_settings.SetKey(st_code, st_key);
             strcpy(glo.m_st_unlock_key, uni(m_ws_key).to_st());
             strcpy(glo.m_st_code, st_key);
             FileConfigSave(&glo);
             PlaySound(_T("media\\secret.wav"), NULL, SND_FILENAME | SND_ASYNC);

		     MessageBox(_T("The program has been successfully unlocked.\n\nA huge thank you for buying it and supporting quality software development!"), _T("Key activated!"), MB_OK);
       
	CBkDialogST::OnOK();
}
