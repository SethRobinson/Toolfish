#if !defined(AFX_DLGEVENTS_H__A7F1341B_B3EC_4DB8_A9A0_3366DF983093__INCLUDED_)
#define AFX_DLGEVENTS_H__A7F1341B_B3EC_4DB8_A9A0_3366DF983093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEvents.h : header file
//
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"
#include "..\Shared\util\TreeCtrlEx.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgEvents dialog

class CDlgEvents : public CBkDialogST

{
// Construction
public:
	CDlgEvents(CWnd* pParent = NULL);   // standard constructor
   void CDlgEvents::DeleteItemsBranch (CTreeCtrlEx & tree, HTREEITEM hitem );
    void CDlgEvents::RunItemsBranch (CTreeCtrlEx & tree, HTREEITEM hitem );
  void CDlgEvents::UpdateEventNamesInBranch (CTreeCtrlEx & tree, HTREEITEM hitem );
  BOOL CDlgEvents::TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop);

   void CDlgEvents::DeleteEvent(int i_event);
	CTreeCtrlEx & GetTreeCtrl(){return m_tree;}
    CImageList m_image_list;
    CImageList & GetImageList(){return m_image_list;}
   void CDlgEvents::UpdateTreeItem(HTREEITEM h_item, int i);
   void CDlgEvents::AddTreeEvent(int i);
    BOOL        m_bDragging;
	HTREEITEM   m_hitemDrag;
	HTREEITEM   m_hitemDrop;
   	CImageList  *m_pimagelist;

// Dialog Data
	//{{AFX_DATA(CDlgEvents)
	enum { IDD = IDD_EVENTS };
	CTreeCtrlEx	m_tree;
	CButtonST	m_but_pic;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEvents)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEvents)
	afx_msg void OnAddevent();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeleteevent();
	afx_msg void OnEditevent();
	afx_msg void OnDblclkEvents(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRunNow();
	afx_msg void OnEnable();
	virtual void OnOK();
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddFolder();
	afx_msg void OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

    void InitEventTree();
    void RebuildEventTree();
    HTREEITEM GetTreeDirectoryHandle(TCHAR p_dir[], bool b_create);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEVENTS_H__A7F1341B_B3EC_4DB8_A9A0_3366DF983093__INCLUDED_)
