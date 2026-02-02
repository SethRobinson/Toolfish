#include "stdafx.h"
#include "CEvent.h"

TCHAR g_buffer[512];
 
#ifndef _UNICODE
char g_event_name_buff[C_EVENT_MAX_NAME_SIZE];
#endif


CEvent::CEvent()
{
    memset(this, 0, sizeof(CEvent));
    wcscpy(m_st_name, L"Name Me!");
    m_f_version = C_F_VERSION;


    this->SetAutoNameEnabled(true);
    InitTrigger();
    InitAction();
    SetEnabled(true);
}


CEvent::~CEvent()
{
   KillTrigger();
   KillAction();
   KillCategory();

}




CTrigger * CEvent::FindTrigger(int i_trigger_type)
{
    int i_max = GetTriggerCount();
    CTrigger *p_trigger = NULL;

    for (int i=0; i < i_max; i++ )
    {
        p_trigger = GetTrigger(i);
        if (p_trigger) if (p_trigger->GetTriggerType() == i_trigger_type)
        {
            return p_trigger;
        }
    }
   return NULL;  //found nothing   
}

CAction * CEvent::FindAction(int i_action_type)
{
    int i_max = GetActionCount();
    CAction *p_action = NULL;

    for (int i=0; i < i_max; i++ )
    {
        p_action = GetAction(i);
        if (p_action) if (p_action->GetActionType() == i_action_type)
        {
            return p_action;
        }
    }
   return NULL;  //found nothing   
}


TCHAR * CEvent::GetAutoName(bool b_long)
{
    //figure out a name that best describes this
    TCHAR ws_trigger[512];
    CTrigger *p_trig = NULL;
  
  
    if (p_trig =FindTrigger(C_TRIGGER_HOTKEY))
    {
        _tcscpy(ws_trigger, p_trig->GetName());
    } else

     if (p_trig = FindTrigger(C_TRIGGER_TIME))
    {
         //if it's a webcompare service too, let's do this to make it stand out more
         _tcscpy(ws_trigger, p_trig->GetName());

         if (FindAction(C_ACTION_COMPARE))
         {
             wsprintf(ws_trigger, _T("Web Compare - %s"), p_trig->GetName());
         }
     } else
     if (p_trig =FindTrigger(C_TRIGGER_INACTIVITY))
    {
        _tcscpy(ws_trigger, p_trig->GetName());
    } else
   if (p_trig =FindTrigger(C_TRIGGER_PROCESS))
    {
        _tcscpy(ws_trigger, p_trig->GetName());
    } else


     {
       _tcscpy(ws_trigger, _T("(No trigger?!)"));
     }
    
     
     //now get the result
     TCHAR ws_action[512];
     CAction *p_action = NULL;
     if (p_action = FindAction(C_ACTION_REBOOT))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
    if (p_action = FindAction(C_ACTION_COMPARE))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
     if (p_action = FindAction(C_ACTION_EMAIL))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
     
     if (p_action = FindAction(C_ACTION_MONITOR))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
     if (p_action = FindAction(C_ACTION_ATOMIC))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
   
 if (p_action = FindAction(C_ACTION_WEBSITE))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
     if (p_action = FindAction(C_ACTION_RUN))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
   if (p_action = FindAction(C_ACTION_VOLUME))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
		 if (p_action = FindAction(C_ACTION_MIC_VOLUME))
		 {
			 _tcscpy(ws_action, p_action->GetName(b_long));
		 } else
    if (p_action = FindAction(C_ACTION_SEND))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else


  if (p_action = FindAction(C_ACTION_MESSAGE))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
     if (p_action = FindAction(C_ACTION_SOUND))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
      if (p_action = FindAction(C_ACTION_STEALTH))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
      if (p_action = FindAction(C_ACTION_TOGGLE))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
       if (p_action = FindAction(C_ACTION_LEET))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
     if (p_action = FindAction(C_ACTION_TTS))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
      if (p_action = FindAction(C_ACTION_SMARTMUTE))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
          if (p_action = FindAction(C_ACTION_CHANGE_REZ))
          {
              _tcscpy(ws_action, p_action->GetName(b_long));
          }
          else

       if (p_action = FindAction(C_ACTION_DELAY))
     {
         _tcscpy(ws_action, p_action->GetName(b_long));
     } else
    



     {
         _tcscpy(ws_action, _T("(No action?!)"));
     }
  
     _stprintf(g_buffer, _T("%s - %s"), ws_trigger, ws_action);

    return (TCHAR*)&g_buffer;
}

WCHAR * CEvent::GetCategoryName()
{
    return m_p_wst_category;
}


//regardless of what we receieve, let's save the data as unicode for win 9/x compatibility.

void CEvent::SetCategoryName(WCHAR *p_text)
{
    //ws version
    //first free what we've got in there
    KillCategory();

    //now reallocate what we need
    m_i_category_size = wcslen(p_text)+1;
    m_p_wst_category = new WCHAR[m_i_category_size];
    wcscpy(m_p_wst_category, p_text);
}

void CEvent::SetCategoryName(CHAR *p_ansi_text)
{
    //char version
    //first free what we've got in there
    KillCategory();

    uni u_text(p_ansi_text);  //convert it to unicode

    //now reallocate what we need
    m_i_category_size = wcslen((WCHAR*)u_text.us_data)+1;
    m_p_wst_category = new WCHAR[m_i_category_size];
    wcscpy(m_p_wst_category, (WCHAR*)u_text.us_data);
}



//this is called when upgrading old data, before we had categories
void CEvent::SetCategoryNameBasedOnEventType()
{
    //default
    SetCategoryName(_T("Events\\Misc\\"));

    if (FindTrigger(C_TRIGGER_HOTKEY))
    {
        SetCategoryName(_T("Events\\Hotkeys\\"));
    } else
    if (FindAction(C_ACTION_COMPARE))
    {
        SetCategoryName(_T("Events\\Web Compares\\"));
    } else
    if (FindAction(C_ACTION_EMAIL))
    {
        SetCategoryName(_T("Events\\Email Checks\\"));
    } else
    if (FindAction(C_TRIGGER_INACTIVITY))
    {
        SetCategoryName(_T("Events\\Activity Based\\"));
    } 
    
    if (FindAction(C_ACTION_MONITOR))
    {
        SetCategoryName(_T("Events\\Site Monitor\\"));
    } 
   

}


void CEvent::InitTrigger()
{
  m_p_vec_trigger = new TriggerVec();
}

void CEvent::KillTrigger()
{
   if (!m_p_vec_trigger) return;

    //kill all items in it too
    for (unsigned int i=0; i < m_p_vec_trigger->size(); i++)
	{
		SAFE_DELETE((*m_p_vec_trigger)[i]);
	}
    
    SAFE_DELETE(m_p_vec_trigger);
}

void CEvent::KillCategory()
{
  SAFE_DELETE_ARRAY(m_p_wst_category);
}


int CEvent::GetTriggerCount()
{
    if ( (!m_p_vec_trigger))
    {
        return 0;
    }   else
    {
        return m_p_vec_trigger->size();
    }

}

CTrigger * CEvent::GetTrigger(int i_index)
{
    if ( (!m_p_vec_trigger) || (int(m_p_vec_trigger->size()) <= i_index) )
    {
        log_error("Trigger %d is invalid.", i_index);
        return NULL;
    }
    
    return (*m_p_vec_trigger)[i_index];
}

int CEvent::AddTrigger()
{
  	CTrigger *p_new_trigger = new CTrigger();
	if (!p_new_trigger)
	{
		log_error("Not enough memory to add a Trigger.");
		return -1;
	}

    m_p_vec_trigger->push_back(p_new_trigger);
   	int i_index = m_p_vec_trigger->size()-1;
    
    return i_index;

}   


void CEvent::DeleteTrigger(int i_index)
{
    if (!GetTrigger(i_index))
   {
       log_error("Tried to delete trigger %d, it doesn't exist.", i_index);
       return;
   }

   SAFE_DELETE((*m_p_vec_trigger)[i_index]);
   (*m_p_vec_trigger)[i_index] = NULL;

   //oh, actually remove it

   m_p_vec_trigger->erase(m_p_vec_trigger->begin()+i_index);

}

int CEvent::MoveTriggerDown(int i_index)
{
    
    ASSERT(GetTrigger(i_index));
    //cause a trigger to be activated a bit later if possible, moves in the array
    //return it's new placement in the array

    //first grab the trigger pointer to save it
    
    CTrigger *p_trig_a =GetTrigger(i_index);
    CTrigger *p_trig_b = NULL;

    //two triggers are going to switch places. but where is the second place?
    int i_index_b = -1;
    for (int i=i_index+1; i < GetTriggerCount(); i++)
    {
     
        if (GetTrigger(i))
        {
            //this is it!
            p_trig_b = GetTrigger(i);
            i_index_b = i;
            break;
        }

    }

    //did we find one?
    if (p_trig_b)
    {
        //yep, let's switch them.
        (*m_p_vec_trigger)[i_index] = p_trig_b;
        (*m_p_vec_trigger)[i_index_b] = p_trig_a;

      return i_index_b;    
    }
    
    //if we got here no change was actually made
    return i_index;
}


int CEvent::MoveTriggerUp(int i_index)
{
    
    ASSERT(GetTrigger(i_index));
    //cause a trigger to be activated a bit later if possible, moves in the array
    //return it's new placement in the array

    //first grab the trigger pointer to save it
    
    CTrigger *p_trig_a =GetTrigger(i_index);
    CTrigger *p_trig_b = NULL;

    //two triggers are going to switch places. but where is the second place?
    int i_index_b = -1;
    for (int i=0; i < i_index; i++)
    {
     
        if (GetTrigger(i))
        {
            //this is it!
            p_trig_b = GetTrigger(i);
            i_index_b = i;
           
        }

    }

    //did we find one?
    if (p_trig_b)
    {
        //yep, let's switch them.
        (*m_p_vec_trigger)[i_index] = p_trig_b;
        (*m_p_vec_trigger)[i_index_b] = p_trig_a;

      return i_index_b;    
    }
    
    //if we got here no change was actually made
    return i_index;
}



int CEvent::MoveActionDown(int i_index)
{
    
    ASSERT(GetAction(i_index));
    //cause a action to be activated a bit later if possible, moves in the array
    //return it's new placement in the array

    //first grab the action pointer to save it
    
    CAction *p_trig_a =GetAction(i_index);
    CAction *p_trig_b = NULL;

    //two actions are going to switch places. but where is the second place?
    int i_index_b = -1;
    for (int i=i_index+1; i < GetActionCount(); i++)
    {
     
        if (GetAction(i))
        {
            //this is it!
            p_trig_b = GetAction(i);
            i_index_b = i;
            break;
        }

    }

    //did we find one?
    if (p_trig_b)
    {
        //yep, let's switch them.
        (*m_p_vec_action)[i_index] = p_trig_b;
        (*m_p_vec_action)[i_index_b] = p_trig_a;

      return i_index_b;    
    }
    
    //if we got here no change was actually made
    return i_index;
}


int CEvent::MoveActionUp(int i_index)
{
    
    ASSERT(GetAction(i_index));
    //cause a action to be activated a bit later if possible, moves in the array
    //return it's new placement in the array

    //first grab the action pointer to save it
    
    CAction *p_trig_a =GetAction(i_index);
    CAction *p_trig_b = NULL;

    //two actions are going to switch places. but where is the second place?
    int i_index_b = -1;
    for (int i=0; i < i_index; i++)
    {
     
        if (GetAction(i))
        {
            //this is it!
            p_trig_b = GetAction(i);
            i_index_b = i;
           
        }

    }

    //did we find one?
    if (p_trig_b)
    {
        //yep, let's switch them.
        (*m_p_vec_action)[i_index] = p_trig_b;
        (*m_p_vec_action)[i_index_b] = p_trig_a;

      return i_index_b;    
    }
    
    //if we got here no change was actually made
    return i_index;
}



void CEvent::InitAction()
{
  m_p_vec_action = new ActionVec();
}

void CEvent::KillAction()
{
    if (!m_p_vec_action) return;
    
    //kill all items in it too
    for (unsigned int i=0; i < m_p_vec_action->size(); i++)
	{
		SAFE_DELETE((*m_p_vec_action)[i]);
	}
     
    SAFE_DELETE(m_p_vec_action);
}

void CEvent::DeleteEventFile()
{
    if (!m_st_filename)
    {
        log_error("Can't delete event's physical file, no filename specified.");
        return;
    }

#ifdef _UNICODE
    _wunlink(m_st_filename);
#else
    unlink(uni(m_st_filename).to_st());

#endif

}

int CEvent::GetActionCount()
{
    if ( (!m_p_vec_action))
    {
        return 0;
    }   else
    {
        return m_p_vec_action->size();
    }

}

CAction * CEvent::GetAction(int i_index)
{
    if ( (!m_p_vec_action) || (int(m_p_vec_action->size()) <= i_index) )
    {
        log_error("Action %d is invalid.", i_index);
        return NULL;
    }
    
    return (*m_p_vec_action)[i_index];
}

int CEvent::AddAction()
{
  	CAction *p_new_action = new CAction();
	if (!p_new_action)
	{
		log_error("Not enough memory to add a Action.");
		return -1;
	}

    m_p_vec_action->push_back(p_new_action);
   	int i_index = m_p_vec_action->size()-1;
    
    return i_index;


}   


void CEvent::DeleteAction(int i_index)
{
    if (!GetAction(i_index))
   {
       log_error("Tried to delete action %d, it doesn't exist.", i_index);
       return;
   }

   SAFE_DELETE((*m_p_vec_action)[i_index]);
   (*m_p_vec_action)[i_index] = NULL;

   //oh, actually remove it

   m_p_vec_action->erase(m_p_vec_action->begin()+i_index);
}

int CEvent::CountActiveTriggers()
{
    int i_count = 0;
    for (unsigned int i=0; i < m_p_vec_trigger->size(); i++)
    {
        //if not null it's valid
        if (GetTrigger(i)) i_count++;
    }

   return i_count;
}


int CEvent::CountActiveActions()
{
    int i_count = 0;
    for (unsigned int i=0; i < m_p_vec_action->size(); i++)
    {
        //if not null it's valid
        if (GetAction(i)) i_count++;
    }

   return i_count;
}


TCHAR * CEvent::GetStatus()
{
    if (this->GetEnabled()) return _T("Active"); else return _T("Disabled");
}

bool CEvent::Save()
{
 if (!m_st_filename)
 {
     log_error("Can't save event, doesn't have a filename yet.");
     return false;
 }
  FILE * fp;

#ifdef _UNICODE
  if ( (fp=_tfopen(m_st_filename, _T("wb"))) == NULL)
#else
  if ( (fp=_tfopen(uni(m_st_filename).to_st(), _T("wb"))) == NULL)

#endif
  {
      log_error("Unable to open %s for saving.",uni(m_st_filename).to_st());
	  return false; //error
	}

//	LogMsg(_T("Saving %s"), m_st_filename);

   m_f_version = C_F_VERSION;

  //first save this structure
   	int i_size = sizeof(CEvent);
  
    //first write the size of this struct
  	fwrite(&i_size, sizeof(int), 1, fp);

    //now write the struct
    if (fwrite(this, i_size, 1, fp) <= 0)
	{
	  fclose(fp);
      log_error("Error writing to %s.",m_st_filename);
      return false;
	}


    //great, but now we have to deal with the triggers
    int i_triggers = CountActiveTriggers();

    //first save how many triggers to save total
      fwrite(&i_triggers, sizeof(int), 1, fp);

      //let the triggers save themselves

      for (unsigned int i_trig = 0; i_trig < m_p_vec_trigger->size(); i_trig++)
      {
          if (GetTrigger(i_trig))
          {
              GetTrigger(i_trig)->Save(fp);
          }
      }

      //same with actions
   
      //great, but now we have to deal with the actions
    int i_action_count= CountActiveActions();

    //first save how many actions to save total
      fwrite(&i_action_count, sizeof(int), 1, fp);


     for (unsigned int i_action = 0; i_action < m_p_vec_action->size(); i_action++)
      {
          if (GetAction(i_action))
          {
              GetAction(i_action)->Save(fp);
          }
      }

     //and finally, save our category info.  
     if (m_i_category_size > 0)
     fwrite(m_p_wst_category, m_i_category_size*2, 1, fp);
     
     fclose(fp);
    return true; //no error
}

bool CEvent::Load(TCHAR p_filename[])
{
    //first free anything that is currently in this..
   KillTrigger();
   KillAction(); 
   KillCategory(); //clear mem we had initted for this
  
   
   //this will have to be initted before using them

    
    FILE * fp;
    if ( (fp=_tfopen(p_filename, _T("rb"))) == NULL)

    {
        log_error("Unable to open %s for loading.",p_filename);
        return false; 
    }
    
   //first save this structure
   	int i_size;
  
    //first read the size of this struct
  	fread(&i_size, sizeof(int), 1, fp);
    if (i_size > sizeof(CEvent))
    {
        fclose(fp);
        LogError(_T("Serious error, event %s is bigger than we can handle, delete it or upgrade Toolfish."), p_filename);
        return false;
    }
   
#ifdef _DEBUG
	if (i_size != sizeof(CEvent))
	{
		LogMsg(_T("Event size changed: Disk: %d, ours: %d"), i_size,
			sizeof(CEvent));
	}
#endif
    //now read the struct
    if (fread(this, i_size, 1, fp) <= 0)
	{

        fclose(fp);
        LogError(_T("Error reading from %s."),p_filename);
        return false;
	}

    //zero out the bad pointers we loaded
    m_p_vec_action = NULL;
    m_p_vec_trigger = NULL;
    m_p_wst_category = NULL;

    //re init them
    this->InitAction();
    this->InitTrigger();

    //now we're ready to add the triggers from the file

    int i_trigger_count = 0;

    //first load how many triggers to save total
    fread(&i_trigger_count, sizeof(int), 1, fp);

	int i;
    for (i=0; i < i_trigger_count; i++)
    {
        int i_index = this->AddTrigger();
        this->GetTrigger(i_index)->Load(fp);
    }

    //same with actions

    int i_action_count = 0;

    //load how many actions to save total
    fread(&i_action_count, sizeof(int), 1, fp);
   
    for (i=0; i < i_action_count; i++)
    {
        int i_index = this->AddAction();
        if (!this->GetAction(i_index)->Load(fp))
		{
			//a serious error has occured..
			LogError(_T("Error reading from %s, the file may be corrupted."),p_filename);
			return false;
		}
    }

    //now load our category name if required
    if (m_i_category_size == 0)
    {
        //old event detected, let's upgrade it!
        SetCategoryNameBasedOnEventType();
    
    }   else
    {
       //first init the memory we'll need
        m_p_wst_category = new WCHAR[m_i_category_size]; //includes NULL
        //actually load the category name into it
        fread(m_p_wst_category, m_i_category_size*2, 1, fp);
 
    }

    fclose(fp);
  	return true;
}

