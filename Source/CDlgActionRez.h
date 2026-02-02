#pragma once
#include "..\Shared\BtnST.h"
#include "..\Shared\BkDialogST.h"

class CAction;

// CDlgActionRez dialog

class CDlgActionRez : public CDialog
{
	DECLARE_DYNAMIC(CDlgActionRez)

public:
	CDlgActionRez(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgActionRez();

	void SetActionPointer(CAction* p_new) { m_p_action = p_new; }
	CAction* m_p_action;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ACTION_REZ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
