#include "stdafx.h"
#include "Maint.h"


void ProcessMaintCheck()
{
    CTime now = CTime::GetCurrentTime();

    if (now.GetDay() != CTime(glo.m_ctime_today).GetDay())
    {
      glo.m_ctime_today = static_cast<time_t>(now.GetTime());
      glo.m_stats_today.Reset();
      AutoSave();
    }


}

void AutoSave()
{
      FileConfigSave(&glo);
}
