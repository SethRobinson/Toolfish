// CDlgActionHotKey.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgActionHotKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgActionHotKey dialog


CDlgActionHotKey::CDlgActionHotKey(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgActionHotKey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgActionHotKey)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgActionHotKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgActionHotKey)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgActionHotKey, CDialog)
	//{{AFX_MSG_MAP(CDlgActionHotKey)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgActionHotKey message handlers
