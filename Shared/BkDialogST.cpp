#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif

#include "BkDialogST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBkDialogST::CBkDialogST(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CBkDialogST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Init();
}

CBkDialogST::CBkDialogST(UINT uResourceID, CWnd* pParent)
	: CDialog(uResourceID, pParent)
{
	Init();
}


CBkDialogST::CBkDialogST(LPCTSTR pszResourceID, CWnd* pParent)
	: CDialog(pszResourceID, pParent)
{
	Init();
}

CBkDialogST::~CBkDialogST()
{
	FreeResources();
}

void CBkDialogST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBkDialogST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBkDialogST, CDialog)
	//{{AFX_MSG_MAP(CBkDialogST)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP

#ifndef UNDER_CE
	ON_WM_LBUTTONDOWN()
#endif

END_MESSAGE_MAP()

void CBkDialogST::Init()
{
	FreeResources(FALSE);

	// Default drawing bitmap mode
	m_byMode = BKDLGST_MODE_TILE;

	// No EasyMove mode
#ifndef UNDER_CE
	m_bEasyMoveMode = FALSE;
#endif
} // End of Init

void CBkDialogST::FreeResources(BOOL bCheckForNULL)
{
	if (bCheckForNULL == TRUE)
	{
		// Destroy bitmap
		if (m_hBitmap)	::DeleteObject(m_hBitmap);
        if (m_hbrush) DeleteObject(m_hbrush);

	} // if

	m_hBitmap = NULL;
    m_hbrush = NULL;
	m_dwWidth = 0;
	m_dwHeight = 0;
} // End of FreeResources

#ifndef UNDER_CE
void CBkDialogST::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// EasyMove mode
	if (m_bEasyMoveMode == TRUE)
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	
	CDialog::OnLButtonDown(nFlags, point);
} // End of OnLButtonDown
#endif

//
// Parameters:
//		[IN]	bActivate
//				TRUE if EasyMove mode must be activated.
//
// Return value:
//		BKDLGST_OK
//			Function executed successfully.
//
#ifndef UNDER_CE
DWORD CBkDialogST::ActivateEasyMoveMode(BOOL bActivate)
{
	m_bEasyMoveMode = bActivate;

	return BKDLGST_OK;
} // End of ActivateEasyMoveMode
#endif

//
// Parameters:
//		[IN]	nBitmap
//				Resource ID of the bitmap to use as background.
//
// Return value:
//		BKDLGST_OK
//			Function executed successfully.
//		BKDLGST_INVALIDRESOURCE
//			The resource specified cannot be found or loaded.
//
DWORD CBkDialogST::SetBitmap(int nBitmap)
{
	HBITMAP		hBitmap			= NULL;
	HINSTANCE	hInstResource	= NULL;

	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);

	// Load bitmap In
	hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmap), IMAGE_BITMAP, 0, 0, 0);

	return SetBitmap(hBitmap);
} // End of SetBitmap

//
// Parameters:
//		[IN]	hBitmap
//				Handle to the bitmap to use as background.
//
// Return value:
//		BKDLGST_OK
//			Function executed successfully.
//		BKDLGST_INVALIDRESOURCE
//			The resource specified cannot be found or loaded.
//
DWORD CBkDialogST::SetBitmap(HBITMAP hBitmap)
{
	int		nRetValue;
	BITMAP	csBitmapSize;

	// Free any loaded resource
	FreeResources();

	if (hBitmap)
	{
		m_hBitmap = hBitmap;

		// Get bitmap size
		nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BKDLGST_INVALIDRESOURCE;
		} // if
		m_dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_dwHeight = (DWORD)csBitmapSize.bmHeight;
	} // if

	Invalidate();

	return BKDLGST_OK;
} // End of SetBitmap



//
DWORD CBkDialogST::SetBitmap(TCHAR *ws_filename)
{
	BITMAP	csBitmapSize;

	// Free any loaded resource
	FreeResources();

		// Get bitmap size
		m_hBitmap = (HBITMAP)LoadImage(NULL, ws_filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (m_hBitmap == 0)
		{
			FreeResources();
			return BKDLGST_INVALIDRESOURCE;
		} // if

        int		nRetValue;

        nRetValue = ::GetObject(m_hBitmap , sizeof(csBitmapSize), &csBitmapSize);
	    if (nRetValue == 0)
        {
         FreeResources();
			return BKDLGST_INVALIDRESOURCE;
   
        }
        m_dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_dwHeight = (DWORD)csBitmapSize.bmHeight;

        //also setup the brush
      	m_hbrush = CreatePatternBrush(m_hBitmap);



        
        Invalidate();

	return BKDLGST_OK;
} // End of SetBitmap



//
// Parameters:
//		[IN]	byMode
//				Specifies how the bitmap will be placed in the dialog background.
//		[IN]	bRepaint
//				If TRUE the dialog will be repainted.
//
// Return value:
//		BKDLGST_OK
//			Function executed successfully.
//		BKDLGST_INVALIDMODE
//			Invalid mode.
//
DWORD CBkDialogST::SetMode(BYTE byMode, BOOL bRepaint)
{
	if (byMode >= BKDLGST_MAX_MODES)	return BKDLGST_INVALIDMODE;

	// Set new mode
	m_byMode = byMode;

	if (bRepaint == TRUE)	Invalidate();

	return BKDLGST_OK;
} // End of SetMode

#ifndef UNDER_CE
DWORD CBkDialogST::ShrinkToFit(BOOL bRepaint)
{
	CRect	rWnd;
	CRect	rClient;
	DWORD	dwDiffCX;
	DWORD	dwDiffCY;

	GetWindowRect(&rWnd);
	GetClientRect(&rClient);

	dwDiffCX = rWnd.Width() - rClient.Width();
	dwDiffCY = rWnd.Height() - rClient.Height();

	m_byMode = BKDLGST_MODE_CENTER;

	MoveWindow(rWnd.left, rWnd.top, dwDiffCX + m_dwWidth, dwDiffCY + m_dwHeight, bRepaint);

	return BKDLGST_OK;
} // End of ShrinkToFit
#endif

BOOL CBkDialogST::OnEraseBkgnd(CDC* pDC) 
{
	CRect		rWnd;
	int			nX			= 0;
	int			nY			= 0;

	BOOL	bRetValue = CDialog::OnEraseBkgnd(pDC);

	// If there is a bitmap loaded
	if (m_hBitmap)
	{
		GetClientRect(rWnd);

		CDC			dcMemoryDC;
		CBitmap		bmpMemoryBitmap;
		CBitmap*	pbmpOldMemoryBitmap = NULL;

		dcMemoryDC.CreateCompatibleDC(pDC);
		bmpMemoryBitmap.CreateCompatibleBitmap(pDC, rWnd.Width(), rWnd.Height());
		pbmpOldMemoryBitmap = (CBitmap*)dcMemoryDC.SelectObject(&bmpMemoryBitmap);

		// Fill background 
		dcMemoryDC.FillSolidRect(rWnd, pDC->GetBkColor());

		CDC			dcTempDC;
		HBITMAP		hbmpOldTempBitmap = NULL;

		dcTempDC.CreateCompatibleDC(pDC);
		hbmpOldTempBitmap = (HBITMAP)::SelectObject(dcTempDC.m_hDC, m_hBitmap);

		switch (m_byMode)
		{
			case BKDLGST_MODE_TILE:
				// Tile the bitmap
				while (nY < rWnd.Height()) 
				{
					while(nX < rWnd.Width()) 
					{
						dcMemoryDC.BitBlt(nX, nY, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
						nX += m_dwWidth;
					} // while
					nX = 0;
					nY += m_dwHeight;
				} // while
				break;
			case BKDLGST_MODE_CENTER:
				nX = ((rWnd.Width() - m_dwWidth)/2);
				nY = ((rWnd.Height() - m_dwHeight)/2);
				dcMemoryDC.BitBlt(nX, nY, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
			case BKDLGST_MODE_STRETCH:
				// Stretch the bitmap
				dcMemoryDC.StretchBlt(0, 0, rWnd.Width(), rWnd.Height(), &dcTempDC, 0, 0, m_dwWidth, m_dwHeight, SRCCOPY);
				break;
			case BKDLGST_MODE_TILETOP:
				while(nX < rWnd.Width()) 
				{
					dcMemoryDC.BitBlt(nX, 0, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
					nX += m_dwWidth;
				} // while
				break;
			case BKDLGST_MODE_TILEBOTTOM:
				while(nX < rWnd.Width()) 
				{
					dcMemoryDC.BitBlt(nX, rWnd.bottom - m_dwHeight, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
					nX += m_dwWidth;
				} // while
				break;
			case BKDLGST_MODE_TILELEFT:
				while (nY < rWnd.Height()) 
				{
					dcMemoryDC.BitBlt(0, nY, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
					nY += m_dwHeight;
				} // while
				break;
			case BKDLGST_MODE_TILERIGHT:
				while (nY < rWnd.Height()) 
				{
					dcMemoryDC.BitBlt(rWnd.right - m_dwWidth, nY, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
					nY += m_dwHeight;
				} // while
				break;
			case BKDLGST_MODE_TOPLEFT:
				dcMemoryDC.BitBlt(0, 0, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
			case BKDLGST_MODE_TOPRIGHT:
				dcMemoryDC.BitBlt(rWnd.right - m_dwWidth, 0, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
			case BKDLGST_MODE_TOPCENTER:
				nX = ((rWnd.Width() - m_dwWidth)/2);
				dcMemoryDC.BitBlt(nX, 0, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
			case BKDLGST_MODE_BOTTOMLEFT:
				dcMemoryDC.BitBlt(0, rWnd.bottom - m_dwHeight, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
			case BKDLGST_MODE_BOTTOMRIGHT:
				dcMemoryDC.BitBlt(rWnd.right - m_dwWidth, rWnd.bottom - m_dwHeight, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
			case BKDLGST_MODE_BOTTOMCENTER:
				nX = ((rWnd.Width() - m_dwWidth)/2);
				dcMemoryDC.BitBlt(nX, rWnd.bottom - m_dwHeight, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);
				break;
		} // switch

		pDC->BitBlt(0, 0, rWnd.Width(), rWnd.Height(), &dcMemoryDC, 0, 0, SRCCOPY);

		//SETH the crash starts with this
		OnPostEraseBkgnd(&dcMemoryDC);

		::SelectObject(dcTempDC.m_hDC, hbmpOldTempBitmap);
		dcMemoryDC.SelectObject(pbmpOldMemoryBitmap);
	} // if

	return bRetValue;
} // End of OnEraseBkgnd

void CBkDialogST::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// If there is a bitmap loaded
	if (m_hBitmap != NULL)
	{
		Invalidate();
	} // if
} // End of OnSize

void CBkDialogST::OnPostEraseBkgnd(CDC* pDC)
{
} // End of OnPostEraseBkgnd
