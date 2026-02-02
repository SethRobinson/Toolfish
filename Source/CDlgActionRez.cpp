// CDlgActionRez.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "CDlgActionRez.h"
#include "afxdialogex.h"
#include "..\Shared\mfc_all.h"
#include "..\Shared\all.h"
#include "..\Shared\uni.h"

#include "CAction.h"

// CDlgActionRez dialog

IMPLEMENT_DYNAMIC(CDlgActionRez, CDialog)



CDlgActionRez::CDlgActionRez(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ACTION_REZ, pParent)
{
	
}

CDlgActionRez::~CDlgActionRez()
{
}

void CDlgActionRez::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgActionRez, CDialog)
END_MESSAGE_MAP()


// CDlgActionRez message handlers
