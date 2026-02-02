// DlgEvents.cpp : implementation file
//

#include "stdafx.h"
#include "Source.h"
#include "DlgEvents.h"
#include "CGlobals.h"
#include "CEventList.h"
#include "CEvent.h"
#include "CDlgEvent.h"       
#include "CDlgOkCancel.h"

#include "ProcessEvents.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int C_EVENT_LIST_NAME = 0;
const int C_EVENT_LIST_STATUS = 1;

const int C_TREE_DIRECTORY = -5; //used to keep track of what is a directory and what isn't for the tree event view

const int C_MAX_CATEGORY_SIZE = 10000; //very large, I know. But I envision webpage spidering in the future.

enum
{
	C_ICON_FOLDER = 0,
	C_ICON_NORMAL,
	C_ICON_DISABLED,
	C_ICON_FOLDER_OPEN,
	C_ICON_FISH_ALARM
};

/////////////////////////////////////////////////////////////////////////////
// CDlgEvents dialog


CDlgEvents::CDlgEvents(CWnd* pParent /*=NULL*/)
: CBkDialogST(CDlgEvents::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEvents)
	//}}AFX_DATA_INIT
}


void CDlgEvents::DoDataExchange(CDataExchange* pDX)
{
	CBkDialogST::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEvents)
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_BUT_PIC, m_but_pic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEvents, CBkDialogST)
	//{{AFX_MSG_MAP(CDlgEvents)
	ON_BN_CLICKED(ID_ADDEVENT, OnAddevent)
	ON_BN_CLICKED(ID_DELETEEVENT, OnDeleteevent)
	ON_BN_CLICKED(ID_EDITEVENT, OnEditevent)
	ON_NOTIFY(NM_DBLCLK, IDC_EVENTS, OnDblclkEvents)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_RUN_NOW, OnRunNow)
	ON_BN_CLICKED(ID_ENABLE, OnEnable)
	ON_BN_CLICKED(ID_DUPLICATE, OnDuplicate)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_BN_CLICKED(ID_ADD_FOLDER, OnAddFolder)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE, OnEndlabeleditTree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEvents message handlers

void CDlgEvents::OnAddevent() 
{
	//Add an event then edit it
	int i_event_id = app_glo.GetEventList()->AddEvent();
	app_glo.GetEventList()->SaveEvent(i_event_id);
	if (i_event_id == -1)  return; //error

	CDlgEvent dlg;
	dlg.SetEventToEdit(app_glo.GetEventList()->GetEvent(i_event_id));
	dlg.DoModal();

	if (
		(app_glo.GetEventList()->GetEvent(i_event_id)->CountActiveActions() == 0)
		&&
		(app_glo.GetEventList()->GetEvent(i_event_id)->CountActiveTriggers() == 0)
		)
	{
		//um, it's totally blank?
		DlgOkCancel dlg;
		dlg.SetTitle(_T("Event has nothing in it"));
		dlg.SetDialogText(_T("The event you just made has no actions or triggers.  Create it anyway?"));
		if (dlg.DoModal() == IDOK)
		{

		} else
		{
			//erase the event
			app_glo.GetEventList()->DeleteEvent(i_event_id);
			i_event_id = -1;
		}
	}

	if (i_event_id != -1)
	{
		//set the default directory it should appear in now?
		TCHAR tc_folder_base[C_MAX_CATEGORY_SIZE];
		GetBaseFolderOfHighlightedItem(GetTreeCtrl(), (TCHAR*)&tc_folder_base, C_TREE_DIRECTORY);
		app_glo.GetEventList()->GetEvent(i_event_id)->SetCategoryName(tc_folder_base);

		//change later to set the events folder base            
		AddTreeEvent(i_event_id); //add it to our visual tree list
	}

}

BOOL CDlgEvents::OnInitDialog() 
{
	CBkDialogST::OnInitDialog();
	m_but_pic.SetTooltipText(_T("This is the heart of Toolfish.  You can create many kinds of custom tools here."));
	m_but_pic.DrawTransparent();
	m_but_pic.SetBitmaps(_T("gui\\but_event.bmp"), RGB(255,255,255), 0,0);
	m_but_pic.SizeToContent();
	m_but_pic.DrawBorder(false, false);
	m_pimagelist = NULL;
	//setup event bar
	if (SetBitmap(_T("gui\\background.bmp")) != BKDLGST_OK)
	{
		log_error("Unable to load dialog background.");
	}

	//let's not allow any events to be run while this dialog is open
	app_glo.SetPaused(true);

	InitEventTree();
	//init event list
	//RebuildEventList();
	m_bDragging = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgEvents::DeleteEvent(int i_event)
{
	//log_msg("Processing %d.", i_event);
	if (i_event >= 0)
	{
		
#ifdef _DEBUG
		LogMsg(_T("Deleting event %d, %s"), i_event, app_glo.GetEventList()->GetEvent(i_event)->GetSmartName());
#endif
		//seems like a valid event, let's kill it.
		if (app_glo.GetEventList()->GetEvent(i_event)) //make sure it exists
		{
			app_glo.GetEventList()->DeleteEvent(i_event);
		}

	}

}


//recursively delete all events on this tree.  **DOESN'T ACTUALLY remove items from the tree itself*

void CDlgEvents::DeleteItemsBranch (CTreeCtrlEx & tree, HTREEITEM hitem )
{
	HTREEITEM hparent, child, next ;
	hparent = hitem ;

	//do it
	GetTreeCtrl().SelectItemEx(hitem, false);
	this->DeleteEvent(tree.GetItemData(hitem)); //delete it if it's not a dir

	if ( ( child = tree.GetChildItem ( hparent ) ) == NULL )
		return;
	do

	{
		DeleteItemsBranch ( tree, child ) ;
		//do it

		GetTreeCtrl().SelectItemEx(hitem, false);
		this->DeleteEvent(tree.GetItemData(hitem)); //delete it if it's not a dir

		next = tree.GetNextSiblingItem ( child ) ;
		child = next ;
	} while ( child != NULL ) ;
}



void CDlgEvents::RunItemsBranch (CTreeCtrlEx & tree, HTREEITEM hitem )
{
	HTREEITEM hparent, child, next ;
	hparent = hitem ;

	//do it
	tree.SelectItemEx(hitem, false);  //deselect it, good for later if it's deleted...
	int i_index = tree.GetItemData(hitem);
	if (i_index != C_TREE_DIRECTORY)
	{
		RunEvent(app_glo.GetEventList()->GetEvent(i_index));
	}      


	if ( ( child = tree.GetChildItem ( hparent ) ) == NULL )
		return;
	do

	{
		RunItemsBranch ( tree, child ) ;
		//do it
		tree.SelectItemEx(hitem, false);  //deselect it, good for later if it's deleted...
		i_index = tree.GetItemData(hitem);
		if (i_index != C_TREE_DIRECTORY)
		{
			RunEvent(app_glo.GetEventList()->GetEvent(i_index));
		}        

		next = tree.GetNextSiblingItem ( child ) ;
		child = next ;
	} while ( child != NULL ) ;
}



void CDlgEvents::OnDeleteevent() 
{
	HTREEITEM h_item = this->GetTreeCtrl().GetFirstSelectedItem();

	if (h_item)
	{

		DlgOkCancel dlg;
		dlg.SetTitle(_T("Are you sure?"));
		dlg.SetDialogText(_T("Are you sure you wish to delete the selected event(s) or folder(s)?  There is no undo."));
		if (dlg.DoModal() == IDOK)
		{

		} else
		{
			return; //no
		}


		while(h_item)
		{

			//before we do anything, grab the next h_item, we can't do it after we delete this one..
			HTREEITEM h_next_item = this->GetTreeCtrl().GetNextSelectedItem(h_item);

			//something is selected
			int i_index = this->GetTreeCtrl().GetItemData(h_item);
			if (i_index == C_TREE_DIRECTORY)
			{
				//special handling for a directory
				//enumerate all files in the dir and kill them

				DeleteItemsBranch(GetTreeCtrl(), h_item);

				//now actually delete the tree branch

				GetTreeCtrl().SelectItemEx(h_item, false);
				GetTreeCtrl().DeleteItem(h_item);


				//avoid a possible crash
				h_next_item = this->GetTreeCtrl().GetFirstSelectedItem();
				

			} else
			{
				if (app_glo.GetEventList()->GetEvent(i_index))
				{
					app_glo.GetEventList()->DeleteEvent(i_index);
					// RebuildEventTree();
					//For speed, just delete the exact item
					HTREEITEM h_item_to_delete = GetTreeItemFromData(GetTreeCtrl(), i_index);
					if (h_item_to_delete)
					{
						GetTreeCtrl().DeleteItem(h_item_to_delete);

					}   else
					{
						LogError(_T("Error deleting item from tree."));
					}

					return;
				}
			}

			h_item = h_next_item; 

		}
	}  else
	{
		// log_msg("Huh?  Choose an event before clicking delete.");
	}


}

void CDlgEvents::OnEditevent() 
{

	HTREEITEM h_item = this->GetTreeCtrl().GetFirstSelectedItem();
	if (h_item != NULL)
	{
		//something is selected
		int i_index = this->GetTreeCtrl().GetItemData(h_item);


		if (i_index == C_TREE_DIRECTORY)
		{
			//handle like a dir
		}   else
		{


			if (app_glo.GetEventList()->GetEvent(i_index))
			{

				CDlgEvent dlg;
				dlg.SetEventToEdit(app_glo.GetEventList()->GetEvent(i_index));
				dlg.DoModal();
				//RebuildEventTree();
				UpdateTreeItem(h_item, i_index);
				//set_selection_by_data(&m_list, i_index);

			}
		}
	}  else
	{
		//  log_msg("Huh?  Choose an event before clicking edit, pal.");
	}



}

void CDlgEvents::OnDblclkEvents(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditevent();

	*pResult = 0;
}

void CDlgEvents::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	BuildHotKeyInfo();
	CBkDialogST::OnClose();
}

void CDlgEvents::OnDestroy() 
{
	app_glo.SetPaused(false);
	app_glo.GetEventList()->SaveEvents();
	CBkDialogST::OnDestroy();
}




HBRUSH CDlgEvents::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBkDialogST::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{

		// TODO: Change any attributes of the DC here
	case CTLCOLOR_STATIC:  

		// 	case CTLCOLOR_EDIT:
		//	case CTLCOLOR_SCROLLBAR:
		//	case CTLCOLOR_LISTBOX:

		if (
			(pWnd->GetDlgCtrlID() == IDC_GROUPBOX)
			||
			(pWnd->GetDlgCtrlID() == IDC_GROUPBOX1)
			||
			(pWnd->GetDlgCtrlID() == IDC_GROUPBOX2)
			)
		{
			// log_msg("Group box detected.");
			pDC->SetBkMode(TRANSPARENT);
			hbr=(HBRUSH) m_hbrush;
			return hbr;
		}

		//pDC->SetTextColor(MY_TEXTCOLOR);
		pDC->SetBkMode(TRANSPARENT);
		hbr=(HBRUSH) GetStockObject(NULL_BRUSH);
		return hbr;
		break;
	case CTLCOLOR_BTN:
		pDC->SetBkMode(TRANSPARENT);
		hbr=(HBRUSH) m_hbrush;
		return hbr;

		break;

	}

	return hbr;
}

void CDlgEvents::OnRunNow() 
{
	HTREEITEM h_item = this->GetTreeCtrl().GetFirstSelectedItem();

	while(h_item)
	{

		//before we do anything, grab the next h_item, we can't do it after we delete this one..
		HTREEITEM h_next_item = this->GetTreeCtrl().GetNextSelectedItem(h_item);


		//something is selected
		int i_index = this->GetTreeCtrl().GetItemData(h_item);
		if (i_index == C_TREE_DIRECTORY)
		{
			//special handling for a directory
			//special handling for a directory
			//enumerate all files in the dir and kill them

			RunItemsBranch(GetTreeCtrl(), h_item);
			//note, if an event is actually deleted after being run..um.. this could cause a problem.

			//avoid a possible crash
			// h_item = this->GetTreeCtrl().GetFirstSelectedItem();
			// if (!h_item) return; //all done
		} else
		{
			if (app_glo.GetEventList()->GetEvent(i_index))
			{
				//let's run it now
				RunEvent(app_glo.GetEventList()->GetEvent(i_index));
				//let's close this so they can see what's going on
				//OnClose();
			}
		}

		h_item = h_next_item; 

	}


}

void CDlgEvents::OnEnable() 
{
	HTREEITEM h_item = this->GetTreeCtrl().GetFirstSelectedItem();

	while(h_item)
	{

		//before we do anything, grab the next h_item, we can't do it after we delete this one..
		HTREEITEM h_next_item = this->GetTreeCtrl().GetNextSelectedItem(h_item);


		//something is selected
		int i_index = this->GetTreeCtrl().GetItemData(h_item);
		if (i_index == C_TREE_DIRECTORY)
		{
			//special handling for a directory
		} else
		{
			if (app_glo.GetEventList()->GetEvent(i_index))
			{
				//let's run it now
				app_glo.GetEventList()->GetEvent(i_index)->SetEnabled(!app_glo.GetEventList()->GetEvent(i_index)->GetEnabled());
				//save the change we just made for the heck of it
				app_glo.GetEventList()->GetEvent(i_index)->Save();

				//update it visually
				UpdateTreeItem(h_item, i_index);

			}
		}

		h_item = h_next_item; 

	}


}

void CDlgEvents::OnDuplicate()
{
	HTREEITEM h_item = this->GetTreeCtrl().GetFirstSelectedItem();
	if (!h_item) return;

	//something is selected
	int i_index = this->GetTreeCtrl().GetItemData(h_item);

	if (i_index == C_TREE_DIRECTORY)
	{
		//can't duplicate a directory
		return;
	}

	CEvent *p_source_event = app_glo.GetEventList()->GetEvent(i_index);
	if (!p_source_event) return;

	//Save the source event's original filename
	WCHAR wst_original_filename[MAX_PATH];
	wcscpy(wst_original_filename, p_source_event->GetFileName());

	//Create a temp filename for the duplication
	WCHAR wst_temp_filename[MAX_PATH];
	wcscpy(wst_temp_filename, L"events\\temp_duplicate.dat");

	//Temporarily set the source event's filename to the temp file
	p_source_event->SetFileName(wst_temp_filename);

	//Save the source event to the temp file
	if (!p_source_event->Save())
	{
		//Restore original filename and abort
		p_source_event->SetFileName(wst_original_filename);
		LogError(_T("Failed to save event for duplication."));
		return;
	}

	//Restore the source event's original filename
	p_source_event->SetFileName(wst_original_filename);

	//Create a new event (this assigns a unique filename automatically)
	int i_new_event_id = app_glo.GetEventList()->AddEvent();
	if (i_new_event_id == -1)
	{
		//Failed to create new event, clean up temp file
		_tremove(_T("events\\temp_duplicate.dat"));
		return;
	}

	CEvent *p_new_event = app_glo.GetEventList()->GetEvent(i_new_event_id);
	if (!p_new_event)
	{
		_tremove(_T("events\\temp_duplicate.dat"));
		return;
	}

	//Save the new event's assigned filename before loading
	WCHAR wst_new_filename[MAX_PATH];
	wcscpy(wst_new_filename, p_new_event->GetFileName());

	//Load the temp file into the new event
	if (!p_new_event->Load(_T("events\\temp_duplicate.dat")))
	{
		//Load failed, delete the new event and temp file
		app_glo.GetEventList()->DeleteEvent(i_new_event_id);
		_tremove(_T("events\\temp_duplicate.dat"));
		LogError(_T("Failed to load duplicated event."));
		return;
	}

	//Restore the new event's unique filename (Load overwrites it)
	p_new_event->SetFileName(wst_new_filename);
	p_new_event->SetIndex(i_new_event_id);

	//Modify the name to append " Copy"
	WCHAR wst_new_name[C_EVENT_MAX_NAME_SIZE];
	const WCHAR *p_old_name = p_new_event->GetName();
	int i_old_len = (int)wcslen(p_old_name);
	const WCHAR wst_copy_suffix[] = L" Copy";
	int i_suffix_len = (int)wcslen(wst_copy_suffix);

	if (i_old_len + i_suffix_len < C_EVENT_MAX_NAME_SIZE)
	{
		//Name fits, just append
		wcscpy(wst_new_name, p_old_name);
		wcscat(wst_new_name, wst_copy_suffix);
	}
	else
	{
		//Truncate the original name to make room for suffix
		int i_max_base_len = C_EVENT_MAX_NAME_SIZE - i_suffix_len - 1;
		wcsncpy(wst_new_name, p_old_name, i_max_base_len);
		wst_new_name[i_max_base_len] = 0;
		wcscat(wst_new_name, wst_copy_suffix);
	}
	p_new_event->SetName(wst_new_name);

	//Save the new event to its assigned filename
	p_new_event->Save();

	//Delete the temp file
	_tremove(_T("events\\temp_duplicate.dat"));

	//Add the new event to the tree
	AddTreeEvent(i_new_event_id);

	//Select the new item in the tree
	HTREEITEM h_new_item = GetTreeItemFromData(GetTreeCtrl(), i_new_event_id);
	if (h_new_item)
	{
		GetTreeCtrl().SelectItem(h_new_item);
	}
}

void CDlgEvents::OnOK() 
{
	// TODO: Add extra validation here
	BuildHotKeyInfo();

	CBkDialogST::OnOK();
}

void CDlgEvents::InitEventTree()
{
	GetImageList().Create(16, 16, ILC_COLOR8 | ILC_MASK, 0, 4);
	if (GetImageList().Add(AfxGetApp()->LoadIcon(IDR_FOLDER)) == -1)
	{
		LogError(_T("Error loading image"));
	}
	if (GetImageList().Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME)) == -1)
	{
		LogError(_T("Error loading image"));
	}
	//C_ICON_NORMAL = 1; //save the index of this


	if (GetImageList().Add(AfxGetApp()->LoadIcon(IDR_FISH_DISABLED)) == -1)
	{
		LogError(_T("Error loading image"));
	}
	//C_ICON_DISABLED = 2; //save the index of this


	if (GetImageList().Add(AfxGetApp()->LoadIcon(IDR_FOLDER_OPEN)) == -1)
	{
		LogError(_T("Error loading image"));
	}
	//C_ICON_FOLDER_OPEN = 3; //save the index of this

	if (GetImageList().Add(AfxGetApp()->LoadIcon(IDR_FISH_ONETIME)) == -1)
	{
		LogError(_T("Error loading image"));
	}
	//C_ICON_FISH_ALARM = 4; //save the index of this


	GetTreeCtrl().SetImageList(&GetImageList(), TVSIL_NORMAL);

	//ok, first things first.  Let's add each event to our tree.
	RebuildEventTree();

	//expand the events folder

	TCHAR t_events[20];
	_tcscpy(t_events, _T("Events\\"));
	GetTreeCtrl().Expand(GetTreeDirectoryHandle(t_events, true), TVE_EXPAND);

}


//Send it a dir like "Animal\Dog\Beagle\" and it will find it in the CTreeCtrl or create it if desired.  Returns null if
//not found (and the b_create flag was not set)

//Don't forget the trailing backslash, this is required or it will crash.  I don't want to check for speed
//reasons here, so make sure you check before you call this!

//If sent "\" or "" it returns TVI_ROOT.

//Note, for speed this does destructive edits to the string in p_dir.  When finished, it will be exactly is
//it was passed, however.    If you pass this a const, get ready for crashing.

HTREEITEM CDlgEvents::GetTreeDirectoryHandle(TCHAR *p_dir, bool b_create)
{
	if (p_dir[1] == 0) return TVI_ROOT;
	if (p_dir[0] == 0) return TVI_ROOT;

	HTREEITEM h_tree_parent = NULL; //GetTreeCtrl().GetRootItem();

	//go through each piece
	TCHAR *p_cur = p_dir; //current place we're pointing
	TCHAR *p_start = p_dir; //point at the start of each word
	while (p_cur[0] != 0)
	{
		if (p_cur[0] == '\\')
		{
			//extract name
			p_cur[0] = 0; //fake a NULL here, now p_start will just be pointing to the word we want.

			// LogMsg(_T("Located directory %s."), p_start);

			HTREEITEM h_tree_item = NULL;

			//check the branches here
			HTREEITEM h_tree_child = GetTreeCtrl().GetChildItem(h_tree_parent);
			bool b_found = false;
			while ( (h_tree_child != NULL) && (!b_found))
			{
				//       LogMsg(_T("Comparing to %s."), GetTreeCtrl().GetItemText(h_tree_child));

				if (
					(GetTreeCtrl().GetItemData(h_tree_child) == C_TREE_DIRECTORY)
					&& (_tcscmp(GetTreeCtrl().GetItemText(h_tree_child), p_start) == 0) 
					)
				{
					//match found.
					//     LogMsg(_T("FOUND %s."), GetTreeCtrl().GetItemText(h_tree_child));
					b_found = true;
					if (p_cur[1] == 0)
					{
						//we're done!
						//fix the damage we did
						p_cur[0] = '\\';
						//         LogMsg(_T("FINISHED WITH %s."), GetTreeCtrl().GetItemText(h_tree_child));
						return h_tree_child; //this is the dir they wanted
					}

					//skip to the next part while leaving h_tree_child with valid info
					break;
				}
				h_tree_child = GetTreeCtrl().GetNextItem(h_tree_child, TVGN_NEXT);
			}

			if (!b_found)
			{
				//no dir was found.
				if (!b_create)
				{
					//we're told not to auto-create it so let's just fail.
					//fix the damage we did
					p_cur[0] = '\\';
					return NULL;

				} else
				{
					//let's create the dir and move on.
					h_tree_child = GetTreeCtrl ().InsertItem (p_start,C_ICON_FOLDER,C_ICON_FOLDER,/*TVI_ROOT*/h_tree_parent , TVI_SORT);
					//set the id to something special so we know it's a dir later
					GetTreeCtrl ().SetItemData(h_tree_child, C_TREE_DIRECTORY);


					//        LogMsg(_T("CREATED %s."), GetTreeCtrl().GetItemText(h_tree_child));

					if (p_cur[1] == 0)
					{
						//we're done!
						//fix the damage we did
						p_cur[0] = '\\';
						//      LogMsg(_T("CREATED AND FINISHED %s."), GetTreeCtrl().GetItemText(h_tree_child));
						return h_tree_child; //this is the dir they wanted
					}
				}
			}

			//If we got here, we found/created the dir but it's not the final dir we're looking for.  Keep going.
			h_tree_parent = h_tree_child; //this is our new parent
			//fix the damage we did
			p_cur[0] = '\\';
			//get ready for next word if required
			p_start = p_cur+1;
		}      
		//advance one
		p_cur++;
	}

	LogError(_T("Error parsing in GetTreeDirectoryHandle. (%s)"), p_dir);

	return TVI_ROOT;
}


void CDlgEvents::RebuildEventTree()
{
	//clear the list
	GetTreeCtrl().DeleteAllItems();

	for (int i=0; i < app_glo.GetEventList()->GetCount(); i++)
	{
		if (app_glo.GetEventList()->GetEvent(i))
		{

			AddTreeEvent(i); //will create the folder it's supposed to be in if required

		}
	}


}

void CDlgEvents::AddTreeEvent(int i)
{

	if (!app_glo.GetEventList()->GetEvent(i)->GetCategoryName())
	{
		//let's generate a category name automatically.  It's missing one.
		app_glo.GetEventList()->GetEvent(i)->SetCategoryNameBasedOnEventType();
	}

#ifdef _UNICODE

	HTREEITEM h_tree_parent = GetTreeDirectoryHandle(app_glo.GetEventList()->GetEvent(i)->GetCategoryName(), true); //create if it doesn't exist
#else
	uni u_cat(app_glo.GetEventList()->GetEvent(i)->GetCategoryName());
	HTREEITEM h_tree_parent = GetTreeDirectoryHandle(u_cat.st_data, true); //create if it doesn't exist

#endif

	HTREEITEM h_new_item = GetTreeCtrl ().InsertItem (app_glo.GetEventList()->GetEvent(i)->GetSmartNameLong(), 1,1, h_tree_parent, TVI_SORT);
	//set it's data index so we can figure out where in the event array it is later
	UpdateTreeItem(h_new_item, i);

}
void CDlgEvents::UpdateTreeItem(HTREEITEM h_item, int i)
{
	GetTreeCtrl ().SetItemData(h_item, i);
	GetTreeCtrl().SetItemText(h_item, app_glo.GetEventList()->GetEvent(i)->GetSmartNameLong());
	//set the icon how we want

	if (!app_glo.GetEventList()->GetEvent(i)->GetEnabled())
	{
		//disabled event
		GetTreeCtrl().SetItemImage(h_item, C_ICON_DISABLED, C_ICON_DISABLED);
	} else
	{

		if (app_glo.GetEventList()->GetEvent(i)->GetAutoDeleteAfterRunning())
		{
			//if it's auto delete, puncuate it
			GetTreeCtrl().SetItemImage(h_item, C_ICON_FISH_ALARM, C_ICON_FISH_ALARM);

		}   else
		{
			GetTreeCtrl().SetItemImage(h_item, C_ICON_NORMAL, C_ICON_NORMAL);
		}
	}


}

void CDlgEvents::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEditevent();

	*pResult = 0;
}






void CDlgEvents::OnAddFolder() 
{
	//let's create a folder! yaaaay
	int i_count = -1;


	TCHAR tc_folder_name[C_MAX_CATEGORY_SIZE];
	TCHAR tc_folder_base[C_MAX_CATEGORY_SIZE];
	TCHAR tc_count[12];


	GetBaseFolderOfHighlightedItem(GetTreeCtrl(), (TCHAR*)&tc_folder_base, C_TREE_DIRECTORY);

try_again:
	i_count++;
	_tcscpy(tc_count, _T(""));
	if (i_count > 0)
	{
		//create a unique dir name
		_stprintf(tc_count, _T("%d"), i_count);
	}
	_stprintf(tc_folder_name, _T("%sNew Folder%s\\"), tc_folder_base, tc_count);

	if (GetTreeDirectoryHandle(tc_folder_name, false) != NULL) goto try_again; //create a unique filename


	//finally create it
	HTREEITEM h_tree_parent = GetTreeDirectoryHandle(tc_folder_name, true); //create if it doesn't exist

	//now let them edit the name
	GetTreeCtrl().SetFocus();
	GetTreeCtrl().SelectItem(h_tree_parent);
	CEdit* pmyEdit = GetTreeCtrl().EditLabel(h_tree_parent);

}

BOOL CDlgEvents::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if (

			(pMsg->wParam == VK_RETURN)
			||
			(pMsg->wParam == VK_ESCAPE)
			)
		{
			//send it to the tree control
			return GetTreeCtrl().PreTranslateMessage(pMsg);
		}

		if (pMsg->wParam == VK_F2)
		{
			GetTreeCtrl().EditLabel(GetTreeCtrl().GetSelectedItem());
		}

		if (pMsg->wParam == VK_DELETE)
		{
			OnDeleteevent();
			return true;
		}

	}

	return CBkDialogST::PreTranslateMessage(pMsg);
}



void CDlgEvents::UpdateEventNamesInBranch (CTreeCtrlEx & tree, HTREEITEM hitem )
{
	HTREEITEM hparent, child, next ;
	hparent = hitem ;


	//do it
	//  tree.SelectItemEx(hitem, false);  //deselect it, good for later if it's deleted...
	int i_index = tree.GetItemData(hitem);
	if (i_index != C_TREE_DIRECTORY)
	{

		//I dynamically release the memory to avoid stack errors.  If this because too slow I'll just
		//change this to be a global buffer or whatever.
		TCHAR *tc_folder_base = new TCHAR[C_MAX_CATEGORY_SIZE];

		GetBaseFolderOfItem(GetTreeCtrl(), (TCHAR*)tc_folder_base, C_TREE_DIRECTORY, hitem);
		app_glo.GetEventList()->GetEvent(i_index)->SetCategoryName(tc_folder_base);
		SAFE_DELETE_ARRAY(tc_folder_base); //release the mem

		//also save it
		app_glo.GetEventList()->SaveEvent(i_index);
		// LogMsg(_T("Moving %s to %s"),app_glo.GetEventList()->GetEvent(i_index)->GetAutoName(false), tc_folder_base );

	}      


	if ( ( child = tree.GetChildItem ( hparent ) ) == NULL )
		return;
	do

	{
		UpdateEventNamesInBranch ( tree, child ) ;
		//do it
		//   tree.SelectItemEx(hitem, false);  //deselect it, good for later if it's deleted...
		i_index = tree.GetItemData(hitem);
		if (i_index != C_TREE_DIRECTORY)
		{
			TCHAR *tc_folder_base = new TCHAR[C_MAX_CATEGORY_SIZE];

			GetBaseFolderOfItem(GetTreeCtrl(), (TCHAR*)tc_folder_base, C_TREE_DIRECTORY, hitem);
			app_glo.GetEventList()->GetEvent(i_index)->SetCategoryName(tc_folder_base);

			SAFE_DELETE_ARRAY(tc_folder_base); //release the mem

			//also save it
			app_glo.GetEventList()->SaveEvent(i_index);
			//  LogMsg(_T("Moving %s to %s"),app_glo.GetEventList()->GetEvent(i_index)->GetAutoName(false), tc_folder_base );

		}        

		next = tree.GetNextSiblingItem ( child ) ;
		child = next ;
	} while ( child != NULL ) ;
}


void CDlgEvents::OnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO     *ptvinfo;

	//	GetTreeCtrl().GetParent()->ShowNotification(pNMHDR, pResult);
	ptvinfo = (TV_DISPINFO *)pNMHDR;
	if (ptvinfo->item.pszText != NULL)
	{
		//actually update our item	    
		ptvinfo->item.mask = TVIF_TEXT;
		GetTreeCtrl().SetItem(&ptvinfo->item);

		if (ptvinfo->item.lParam != C_TREE_DIRECTORY)
		{
			//they just renamed a whole item.  Let's update it.
#ifdef _UNICODE
			app_glo.GetEventList()->GetEvent(ptvinfo->item.lParam)->SetName(ptvinfo->item.pszText);
#else
			//convert to unicode
			uni u_temp(ptvinfo->item.pszText);
			app_glo.GetEventList()->GetEvent(ptvinfo->item.lParam)->SetName(u_temp.us_data);
#endif

			//tell the event we want to use custom names
			app_glo.GetEventList()->GetEvent(ptvinfo->item.lParam)->SetAutoNameEnabled(false);

		} else
		{

			//they renamed a directory.  Update all its contents accordingly.
			UpdateEventNamesInBranch(GetTreeCtrl(), ptvinfo->item.hItem);

		}

	}
	*pResult = TRUE;
}

void CDlgEvents::OnBegindragTree(NMHDR* pnmhdr, LRESULT* pLResult) 
{
	CPoint      ptAction;
	UINT        nFlags;

	GetCursorPos(&ptAction);
	GetTreeCtrl().ScreenToClient(&ptAction);
	//SetCursor()

	//  GetTreeCtrl().ShowNotification(pnmhdr, pLResult);
	ASSERT(!m_bDragging);
	m_bDragging = TRUE;
	m_hitemDrag = GetTreeCtrl().HitTest(ptAction, &nFlags);
	m_hitemDrop = NULL;
	int i_event = GetTreeCtrl().GetItemData(m_hitemDrag);

	ASSERT(m_pimagelist == NULL);


	m_pimagelist = GetTreeCtrl().CreateDragImage(m_hitemDrag);  // get the image list for dragging
	m_pimagelist->DragShowNolock(TRUE);
	m_pimagelist->SetDragCursorImage(0, CPoint(0, 0));
	m_pimagelist->BeginDrag(0, CPoint(0,0));
	m_pimagelist->DragMove(ptAction);
	m_pimagelist->DragEnter(this, ptAction);


	SetCapture();
}


BOOL CDlgEvents::TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop)
{
	TV_INSERTSTRUCT     tvstruct;
	TCHAR               sztBuffer[256];
	HTREEITEM           hNewItem, hFirstChild;

	// avoid an infinite recursion situation
	tvstruct.item.hItem = hitemDrag;
	tvstruct.item.lParam = GetTreeCtrl().GetItemData(hitemDrag);
	tvstruct.item.cchTextMax = 256;
	tvstruct.item.pszText = sztBuffer;
	tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	GetTreeCtrl().GetItem(&tvstruct.item);  // get information of the dragged element
	tvstruct.hParent = hitemDrop;
	tvstruct.hInsertAfter = TVI_SORT;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	hNewItem = GetTreeCtrl().InsertItem(&tvstruct);

	while ((hFirstChild = GetTreeCtrl().GetChildItem(hitemDrag)) != NULL)
	{
		TransferItem(hFirstChild, hNewItem);  // recursively transfer all the items
		GetTreeCtrl().DeleteItem(hFirstChild);        // delete the first child and all its children
	}
	return TRUE;
}


void CDlgEvents::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{

		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragLeave(this);
		m_pimagelist->EndDrag();
		delete m_pimagelist;
		m_pimagelist = NULL;


		//  log_msg("Button up!");
		ReleaseCapture();
		m_bDragging = FALSE;
		if (!m_hitemDrop)
		{
			//something wrong, this should never be null
			GetTreeCtrl().SelectDropTarget(NULL);
			return;
		}

		if (m_hitemDrag != m_hitemDrop && !GetTreeCtrl().IsChildNodeOf(m_hitemDrop, m_hitemDrag) &&
			GetTreeCtrl().GetParentItem(m_hitemDrag) != m_hitemDrop)
		{

			TCHAR t_path[C_MAX_CATEGORY_SIZE];
			//add it to the new cat
			int i_event = -1;
			if ((i_event = GetTreeCtrl().GetItemData(m_hitemDrag)) == C_TREE_DIRECTORY)
			{
				//handle like a directory

				//make sure where we're dragging TO is a directory and not a file
				if (GetTreeCtrl().GetItemData(m_hitemDrop) != C_TREE_DIRECTORY)
				{
					m_hitemDrop = GetTreeCtrl().GetParentItem(m_hitemDrop);
				}

				if ( 
					(m_hitemDrop != NULL) && 
					(GetTreeCtrl().GetItemData(m_hitemDrop) == C_TREE_DIRECTORY)
					)
				{

					TransferItem(m_hitemDrag, m_hitemDrop);

					//now let the target process the new dir names for all the events inside.  This could 
					//potentially be slow
					this->UpdateEventNamesInBranch(GetTreeCtrl(), m_hitemDrop);
					//delete the old listing
					GetTreeCtrl().DeleteItem(m_hitemDrag);
				} else
				{
					LogError(_T("Couldn't perform drag and drop, target not a directory."));
				}

			}   else
			{
				//handle like a single event thingie
				if (GetBaseFolderOfItem(GetTreeCtrl(), t_path, C_TREE_DIRECTORY, m_hitemDrop))
				{
					//add the ending //
					//  _tcscat(t_path, _T("\\"));
				}
				//set new path
				app_glo.GetEventList()->GetEvent(i_event)->SetCategoryName(t_path);

				//add it
				AddTreeEvent(i_event);

				//delete the old listing
				GetTreeCtrl().DeleteItem(m_hitemDrag);

			}
		}

		//fix is to selections work again
		// GetTreeCtrl().ClearSelection(true);
		GetTreeCtrl().SelectDropTarget(NULL);
	}
	CBkDialogST::OnLButtonUp(nFlags, point);
}

void CDlgEvents::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM           hitem;
	UINT                flags;

	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragMove(point);


		// GetTreeCtrl().ScreenToClient(&point);
		RECT rect, main;
		GetTreeCtrl().GetWindowRect(&rect);
		GetWindowRect(&main);
		CPoint p_orig = point;
		point.Offset(main.left-rect.left, main.top-(rect.top-20));

		if ((hitem = GetTreeCtrl().HitTest(point, &flags)) != NULL)
		{
			m_pimagelist->DragLeave(this);


			//if we show this.. it loses our selection which isn't good.  So we won't, the problem
			//is CTreeCtrl wasn't designed for multiple selections, but we've subclasses it to
			//provide this ability.

			GetTreeCtrl().SelectDropTarget(hitem);
			m_hitemDrop = hitem;
			m_pimagelist->DragEnter(this, p_orig);


		}
	}	
	CBkDialogST::OnMouseMove(nFlags, point);
}
