#include "Maint.h"


void ProcessMaintCheck()
{
    CTime now = CTime::GetCurrentTime();

    if (now.GetDay() != CTime(glo.m_ctime_today).GetDay())
    {
      glo.m_ctime_today =  *(__time64_t*)&now;
      glo.m_stats_today.Reset();
      AutoSave();
    }


}

void AutoSave()
{
      FileConfigSave(&glo);
}
