#include "stdafx.h"
#include "CEventList.h"
#include "CEvent.h"



CEventList::CEventList()
{
    memset(this, 0, sizeof(CEventList));

    m_p_vec_event = new EventVec();

}


CEventList::~CEventList()
{
    KillEvents();
    SAFE_DELETE(m_p_vec_event);
}

void CEventList::KillEvents()
{
   	for (unsigned long i=0; i < m_p_vec_event->size(); i++)
	{
		SAFE_DELETE((*m_p_vec_event)[i]);
	}

	m_p_vec_event->clear();
}

int CEventList::GetCount()
{
    if (m_p_vec_event) return m_p_vec_event->size(); else return 0;
}

CEvent * CEventList::GetEvent(int i_index)
{
    if ( (!m_p_vec_event) || (int(m_p_vec_event->size()) <= i_index))
    {
        log_error("GetEvent says event %d is out of range.", i_index);
        return 0;
    }

    return (*m_p_vec_event)[i_index];
}

//a return value of -1 indicates an error, otherwise it's the index of the event created
int CEventList::AddEvent()
{
  
    	CEvent *p_new_event = new CEvent();
	if (!p_new_event)
	{
		log_error("Not enough memory to add an event.");
		return -1;
	}

    //give it a unique filename
    char st_temp[MAX_PATH];
    for (int i=0; i < 20000; i++)
    {
         sprintf(st_temp, "events\\event_%d.dat", i) ;
        if (!exist(st_temp))
        {
            //we found the name we can use
            break;
        }

    }

   
    m_p_vec_event->push_back(p_new_event);
   	int i_index = m_p_vec_event->size()-1;
    
    GetEvent(i_index)->SetIndex(i_index);
     //assign this filename
     GetEvent(i_index)->SetFileName(uni(st_temp).GetAuto());
     //save it now so if another event is created it will know this filename is taken
     return i_index;
}   

void CEventList::DeleteEvent(int i_index)
{
   if (!GetEvent(i_index))
   {
       log_error("Tried to delete event %d, it doesn't exist.", i_index);
       return;
   }

   //first delete the file
   GetEvent(i_index)->DeleteEventFile();

   SAFE_DELETE((*m_p_vec_event)[i_index]);

   //item deleted and NULLED.  However, we'll leave the spot empty.  New items will go
   //on the end.  Next time it's reloaded it will fill all gaps though.

    
}

void CEventList::SaveEvents()
{
    //we could save them all to one file, but then it might get to slow to
    //update a particular event after it's been run or whatever, due to the
    //dynamic file sizes.  (one might hold a 50k file in it, one nothing)

    //so, we save them out to custom file names.  If any new actions are found at
    //startup sitting the in the dir (*.dat) they will get loaded too.
 
    for (int i=0; i < GetCount(); i++)
    {
        if (GetEvent(i)) SaveEvent(i);
    }
}

bool CEventList::SaveEvent(int i_index)
{  
    return GetEvent(i_index)->Save();
}

void CEventList::LoadEvents()
{
  KillEvents();
	
	 //be careful, this command changes the dir so we'll be in data\\ until the last
	//call moves it back.
	 TCHAR st_temp[MAX_PATH];																		  
    while (get_files_with_wildard(_T("events\\"),_T("*.dat"), st_temp))
	 {
	
		 
       //files existed
        //first add it, then make it load it
        int i_index = AddEvent();
        //LogMsg(st_temp);
        if (!GetEvent(i_index)->Load(st_temp))
        {
            //error loading this..
            DeleteEvent(i_index);
            continue;
        }
        
        GetEvent(i_index)->SetIndex(i_index); //let it know who it is in the overal index we have
         TCHAR st_name[MAX_PATH];
         wsprintf(st_name, _T("events\\%s"), st_temp);
         GetEvent(i_index)->SetFileName(uni(st_name).GetAuto()); //this way we can save it back out the way we got it
	 }


}



