#include "mute_util.h"
#include "mmsystem.h"
#include "CDlgMute.h"
/*
	Msg("Aux devices is %d.",(int)auxGetNumDevs());
	
  	ULONG mmin,mmax;
		Msg("Volume is %d.\n", (int)GetMasterVolume(mmin,mmax));
	  Msg("Min is %d, max is %d.", (int)mmin, (int)mmax);
      */


void ProcessMuteKey()
{
    if (app_glo.GetMuted())
    {
        //unmute!
        DeactivateMute();
      
    }
   app_glo.ResetMuteTimer();
}

void ProcessMuteMouse()
{
   // log_msg("got mouse");
   // OutputDebugString(_T("Got mouse\r\n"));
    if (app_glo.GetMuted())
    {
         if (app_glo.MouseMoved())
        {
        
          DeactivateMute();
        }
    }

    app_glo.ResetMuteTimer();

}

unsigned long GetMasterVolume(unsigned long &min, unsigned long &max)
{
    unsigned long result = 0;
 
    // open the mixer device and grab its handle
    HMIXER HMixer2;
    if (mixerOpen(&HMixer2, 0, 0, 0, 0) == MMSYSERR_NOERROR)     {
        // initialize the MIXERCOTNROL structure
        MIXERCONTROL mc2;
        ZeroMemory(&mc2, sizeof(MIXERCONTROL));
        mc2.cbStruct = sizeof(MIXERCONTROL);
 
        // initialize the MIXERLINE structure
        MIXERLINE ml2;
        ZeroMemory(&ml2, sizeof(MIXERLINE));
        ml2.cbStruct = sizeof(MIXERLINE);
ml2.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
 
        // fill the mixerline info
if (mixerGetLineInfo((HMIXEROBJ) HMixer2, &ml2, 
                            MIXER_GETLINEINFOF_COMPONENTTYPE)
            == MMSYSERR_NOERROR)
        {
            // initialize the MIXERLINECONTROL structure
            MIXERLINECONTROLS mlc2;
            ZeroMemory(&mlc2, sizeof(MIXERLINECONTROLS));
    mlc2.cbStruct = sizeof(MIXERLINECONTROLS);
    mlc2.dwLineID = ml2.dwLineID;
        mlc2.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
        mlc2.cControls = 1;
    mlc2.cbmxctrl = sizeof(MIXERCONTROL);
    mlc2.pamxctrl = (LPMIXERCONTROL)&mc2;
 
            // find the level display associated with this source      
	if (mixerGetLineControls((HMIXEROBJ)HMixer2, &mlc2,
        MIXER_GETLINECONTROLSF_ONEBYTYPE)   == MMSYSERR_NOERROR)
            {
                // fill in the volume range values
                min = mc2.Bounds.lMinimum;
                max = mc2.Bounds.lMaximum;
 
                // initialize the MIXERCONTROLDETAILS structure
                
				MIXERCONTROLDETAILS mcd2;
                
				ZeroMemory(&mcd2, sizeof(mcd2));
        mcd2.cbStruct = sizeof(MIXERCONTROLDETAILS);
  mcd2.dwControlID = mc2.dwControlID;
  mcd2.cChannels = 1;
      mcd2.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
 
              MIXERCONTROLDETAILS_UNSIGNED mcdMeter2;
      mcd2.paDetails = &mcdMeter2;
 
                // get the peak level of the volume meter
                mixerGetControlDetails((HMIXEROBJ)HMixer2, &mcd2,
                                      MIXER_GETCONTROLDETAILSF_VALUE); 
 
                result = mcdMeter2.dwValue;
            }
    }
    }
    mixerClose(HMixer2);
    return result;
}
 
void SetMasterVolume(unsigned long value)
{
    // open the mixer device and grab its handle
    HMIXER HMixer2;
    if (mixerOpen(&HMixer2, 0, 0, 0, 0) == MMSYSERR_NOERROR)     {
        // initialize the MIXERCOTNROL structure
        MIXERCONTROL mc2;
        ZeroMemory(&mc2, sizeof(MIXERCONTROL));
        mc2.cbStruct = sizeof(MIXERCONTROL);
 
        // initialize the MIXERLINE structure
        MIXERLINE ml2;
        ZeroMemory(&ml2, sizeof(MIXERLINE));
        ml2.cbStruct = sizeof(MIXERLINE);
ml2.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
 
        // fill the mixerline info
if (mixerGetLineInfo((HMIXEROBJ)HMixer2, &ml2, 
                            MIXER_GETLINEINFOF_COMPONENTTYPE)
        == MMSYSERR_NOERROR)
        {
            // initialize the MIXERLINECONTROL structure
            MIXERLINECONTROLS mlc2;
            ZeroMemory(&mlc2, sizeof(MIXERLINECONTROLS));
    mlc2.cbStruct = sizeof(MIXERLINECONTROLS);
    mlc2.dwLineID = ml2.dwLineID;
        mlc2.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
        mlc2.cControls = 1;
    mlc2.cbmxctrl = sizeof(MIXERCONTROL);
    mlc2.pamxctrl = (LPMIXERCONTROL)&mc2;
 
            // find the level display associated with this source         
    if (mixerGetLineControls((HMIXEROBJ)HMixer2, &mlc2,
          MIXER_GETLINECONTROLSF_ONEBYTYPE)
            == MMSYSERR_NOERROR)
            {
                // initialize the MIXERCONTROLDETAILS structure
            MIXERCONTROLDETAILS mcd2;
                ZeroMemory(&mcd2, sizeof(mcd2));
  mcd2.cbStruct = sizeof(MIXERCONTROLDETAILS);
        mcd2.dwControlID = mc2.dwControlID;
  mcd2.cChannels = 1;
      mcd2.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
 
            MIXERCONTROLDETAILS_UNSIGNED mcdMeter2;
      mcd2.paDetails = &mcdMeter2;
 
                // set the new volume value
                mcdMeter2.dwValue = value;
                mixerSetControlDetails((HMIXEROBJ)HMixer2, &mcd2,
                                      MIXER_GETCONTROLDETAILSF_VALUE);     
	}
    }
    }
    mixerClose(HMixer2);
}
 

