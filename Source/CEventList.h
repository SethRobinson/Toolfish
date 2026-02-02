#pragma once

class CEvent; //forward for speed
#include <vector>

using namespace std;

typedef vector<CEvent*> EventVec;


class CEventList
{
public:
    CEventList();
    ~CEventList();
        
    int AddEvent();
    void KillEvents();
    int GetCount();

    CEvent * GetEvent(int i_index);
    void DeleteEvent(int i_index);
    void SaveEvents();
    bool SaveEvent(int i_index); //save just one
    void LoadEvents();
private:
    
    EventVec *m_p_vec_event;
    
};
