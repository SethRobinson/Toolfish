#include "stdafx.h"
#include "mute_util.h"
#include "mmsystem.h"
#include "CDlgMute.h"
#include <mmdeviceapi.h>
#include <endpointvolume.h>

// GUIDs for Core Audio API
static const GUID CLSID_MMDeviceEnumerator_local = { 0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E} };
static const GUID IID_IMMDeviceEnumerator_local = { 0xA95664D2, 0x9614, 0x4F35, {0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6} };
static const GUID IID_IAudioEndpointVolume_local = { 0x5CDF2C82, 0x841E, 0x4546, {0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A} };

// Helper function to get the audio endpoint volume interface
static IAudioEndpointVolume* GetAudioEndpointVolume()
{
    HRESULT hr;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pVolume = NULL;

    hr = CoCreateInstance(CLSID_MMDeviceEnumerator_local, NULL, CLSCTX_ALL,
                          IID_IMMDeviceEnumerator_local, (void**)&pEnumerator);
    if (FAILED(hr) || !pEnumerator)
        return NULL;

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    pEnumerator->Release();
    if (FAILED(hr) || !pDevice)
        return NULL;

    hr = pDevice->Activate(IID_IAudioEndpointVolume_local, CLSCTX_ALL, NULL, (void**)&pVolume);
    pDevice->Release();
    if (FAILED(hr))
        return NULL;

    return pVolume;
}

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

// Volume knob sensitivity - adds extra volume change when volume keys are detected
bool ProcessVolumeKnobKey(WPARAM wParam)
{
    // Only handle volume up/down keys
    if (wParam != VK_VOLUME_UP && wParam != VK_VOLUME_DOWN)
        return false;

    // Check if feature is enabled
    if (!glo.m_b_volume_knob_sensitivity)
        return false;

    // Check if shift-only mode is enabled and shift is not pressed
    if (glo.m_b_volume_knob_shift_only)
    {
        if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
            return false;
    }

    // Calculate extra volume change
    // Each step adds ~2% volume on top of the normal keypress
    int extra_steps = glo.m_i_volume_knob_multiplier;
    if (extra_steps <= 0)
        return false;

    float extra_change = (float)extra_steps * 0.02f;  // 2% per step

    IAudioEndpointVolume* pVolume = GetAudioEndpointVolume();
    if (!pVolume)
        return false;

    float current_vol = 0.0f;
    pVolume->GetMasterVolumeLevelScalar(&current_vol);

    float new_vol;
    if (wParam == VK_VOLUME_UP)
    {
        new_vol = current_vol + extra_change;
        if (new_vol > 1.0f) new_vol = 1.0f;
    }
    else // VK_VOLUME_DOWN
    {
        new_vol = current_vol - extra_change;
        if (new_vol < 0.0f) new_vol = 0.0f;
    }

    pVolume->SetMasterVolumeLevelScalar(new_vol, NULL);
    pVolume->Release();

    return true;
}

// Get master volume using Windows Core Audio API
// Returns volume as 0-65535 for backwards compatibility
// min is always 0, max is always 65535
unsigned long GetMasterVolume(unsigned long &min, unsigned long &max)
{
    min = 0;
    max = 65535;

    IAudioEndpointVolume* pVolume = GetAudioEndpointVolume();
    if (!pVolume)
        return 0;

    float level = 0.0f;
    pVolume->GetMasterVolumeLevelScalar(&level);
    pVolume->Release();

    // Convert 0.0-1.0 to 0-65535
    return (unsigned long)(level * 65535.0f);
}
 
// Set master volume using Windows Core Audio API
// Takes value as 0-65535 for backwards compatibility
void SetMasterVolume(unsigned long value)
{
    IAudioEndpointVolume* pVolume = GetAudioEndpointVolume();
    if (!pVolume)
        return;

    // Convert 0-65535 to 0.0-1.0
    float level = (float)value / 65535.0f;
    if (level > 1.0f) level = 1.0f;
    if (level < 0.0f) level = 0.0f;

    pVolume->SetMasterVolumeLevelScalar(level, NULL);
    pVolume->Release();
}
