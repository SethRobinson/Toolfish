#include "stdafx.h"
#include "gamepad_util.h"
#include <xinput.h>

// DirectInput for PS5/generic controller support
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>

// Maximum number of XInput controllers
#define XUSER_MAX_COUNT 4

// Maximum DirectInput devices to track
#define MAX_DINPUT_DEVICES 8

// ============================================================================
// XInput state tracking (Xbox controllers)
// ============================================================================
static DWORD g_dwLastPacketNumber[XUSER_MAX_COUNT] = {0, 0, 0, 0};
static WORD g_wLastButtons[XUSER_MAX_COUNT] = {0, 0, 0, 0};

// Mask for all digital buttons we care about (excludes analog sticks/triggers)
static const WORD XINPUT_BUTTON_MASK = 
    XINPUT_GAMEPAD_DPAD_UP |
    XINPUT_GAMEPAD_DPAD_DOWN |
    XINPUT_GAMEPAD_DPAD_LEFT |
    XINPUT_GAMEPAD_DPAD_RIGHT |
    XINPUT_GAMEPAD_START |
    XINPUT_GAMEPAD_BACK |
    XINPUT_GAMEPAD_LEFT_THUMB |
    XINPUT_GAMEPAD_RIGHT_THUMB |
    XINPUT_GAMEPAD_LEFT_SHOULDER |
    XINPUT_GAMEPAD_RIGHT_SHOULDER |
    XINPUT_GAMEPAD_A |
    XINPUT_GAMEPAD_B |
    XINPUT_GAMEPAD_X |
    XINPUT_GAMEPAD_Y;

// ============================================================================
// DirectInput state tracking (PS5, generic controllers)
// ============================================================================
static LPDIRECTINPUT8 g_pDirectInput = nullptr;
static std::vector<LPDIRECTINPUTDEVICE8> g_diDevices;
static std::vector<DIJOYSTATE2> g_diLastState;
static HINSTANCE g_hInstance = nullptr;
static bool g_bInitialized = false;
static DWORD g_dwLastEnumTime = 0;
static const DWORD ENUM_INTERVAL_MS = 5000;  // Re-enumerate devices every 5 seconds

// ============================================================================
// DirectInput device enumeration callback
// ============================================================================
static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
    if (!g_pDirectInput || g_diDevices.size() >= MAX_DINPUT_DEVICES)
        return DIENUM_STOP;

    // Check if we already have this device (by GUID)
    for (size_t i = 0; i < g_diDevices.size(); i++)
    {
        DIDEVICEINSTANCE existingInfo;
        existingInfo.dwSize = sizeof(DIDEVICEINSTANCE);
        if (SUCCEEDED(g_diDevices[i]->GetDeviceInfo(&existingInfo)))
        {
            if (IsEqualGUID(existingInfo.guidInstance, pdidInstance->guidInstance))
            {
                // Already have this device
                return DIENUM_CONTINUE;
            }
        }
    }

    // Create the device
    LPDIRECTINPUTDEVICE8 pDevice = nullptr;
    HRESULT hr = g_pDirectInput->CreateDevice(pdidInstance->guidInstance, &pDevice, nullptr);
    if (FAILED(hr) || !pDevice)
        return DIENUM_CONTINUE;

    // Set the data format to joystick
    hr = pDevice->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr))
    {
        pDevice->Release();
        return DIENUM_CONTINUE;
    }

    // Set cooperative level - non-exclusive background access
    // This allows the game to also use the controller
    hr = pDevice->SetCooperativeLevel(nullptr, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr))
    {
        pDevice->Release();
        return DIENUM_CONTINUE;
    }

    // Acquire the device
    hr = pDevice->Acquire();
    if (FAILED(hr))
    {
        // May fail if another app has exclusive access, but we can try again later
    }

    // Add to our device list
    g_diDevices.push_back(pDevice);
    
    // Initialize last state for this device
    DIJOYSTATE2 initialState;
    ZeroMemory(&initialState, sizeof(DIJOYSTATE2));
    g_diLastState.push_back(initialState);

    return DIENUM_CONTINUE;
}

// ============================================================================
// DirectInput initialization
// ============================================================================
static void InitDirectInput()
{
    if (g_pDirectInput)
        return;  // Already initialized

    HINSTANCE hInst = g_hInstance;
    if (!hInst)
    {
        hInst = GetModuleHandle(nullptr);
    }

    HRESULT hr = DirectInput8Create(
        hInst,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&g_pDirectInput,
        nullptr
    );

    if (FAILED(hr))
    {
        g_pDirectInput = nullptr;
    }
}

// ============================================================================
// Enumerate DirectInput devices (finds new controllers)
// ============================================================================
static void EnumerateDirectInputDevices()
{
    if (!g_pDirectInput)
        return;

    g_pDirectInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL,
        EnumJoysticksCallback,
        nullptr,
        DIEDFL_ATTACHEDONLY
    );
}

// ============================================================================
// Check XInput controllers for activity
// ============================================================================
static bool CheckXInputActivity()
{
    bool bActivityDetected = false;

    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        DWORD dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS)
        {
            WORD wCurrentButtons = state.Gamepad.wButtons & XINPUT_BUTTON_MASK;

            if (wCurrentButtons != g_wLastButtons[i])
            {
                bActivityDetected = true;
            }

            g_dwLastPacketNumber[i] = state.dwPacketNumber;
            g_wLastButtons[i] = wCurrentButtons;
        }
        else
        {
            g_dwLastPacketNumber[i] = 0;
            g_wLastButtons[i] = 0;
        }
    }

    return bActivityDetected;
}

// ============================================================================
// Check DirectInput controllers for activity
// ============================================================================
static bool CheckDirectInputActivity()
{
    if (!g_pDirectInput)
        return false;

    bool bActivityDetected = false;

    // Check for disconnected devices and remove them
    for (size_t i = 0; i < g_diDevices.size(); )
    {
        HRESULT hr = g_diDevices[i]->Poll();
        if (FAILED(hr))
        {
            // Try to reacquire
            hr = g_diDevices[i]->Acquire();
            if (FAILED(hr))
            {
                // Device is gone, remove it
                g_diDevices[i]->Release();
                g_diDevices.erase(g_diDevices.begin() + i);
                g_diLastState.erase(g_diLastState.begin() + i);
                continue;
            }
            hr = g_diDevices[i]->Poll();
        }

        if (SUCCEEDED(hr))
        {
            DIJOYSTATE2 state;
            ZeroMemory(&state, sizeof(DIJOYSTATE2));
            hr = g_diDevices[i]->GetDeviceState(sizeof(DIJOYSTATE2), &state);

            if (SUCCEEDED(hr))
            {
                // Check buttons (up to 128 buttons supported)
                for (int btn = 0; btn < 128; btn++)
                {
                    if (state.rgbButtons[btn] != g_diLastState[i].rgbButtons[btn])
                    {
                        bActivityDetected = true;
                        break;
                    }
                }

                // Check POV hat switches (D-pad on many controllers)
                for (int pov = 0; pov < 4; pov++)
                {
                    if (state.rgdwPOV[pov] != g_diLastState[i].rgdwPOV[pov])
                    {
                        bActivityDetected = true;
                        break;
                    }
                }

                // Update last state
                g_diLastState[i] = state;
            }
        }

        i++;
    }

    return bActivityDetected;
}

// ============================================================================
// Public API
// ============================================================================

void InitGamepadTracking(HINSTANCE hInstance)
{
    g_hInstance = hInstance;

    // Initialize XInput tracking state
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        g_dwLastPacketNumber[i] = 0;
        g_wLastButtons[i] = 0;
    }

    // Initialize DirectInput
    InitDirectInput();
    if (g_pDirectInput)
    {
        EnumerateDirectInputDevices();
    }

    g_dwLastEnumTime = GetTickCount();
    g_bInitialized = true;
}

void ShutdownGamepadTracking()
{
    // Release all DirectInput devices
    for (size_t i = 0; i < g_diDevices.size(); i++)
    {
        if (g_diDevices[i])
        {
            g_diDevices[i]->Unacquire();
            g_diDevices[i]->Release();
        }
    }
    g_diDevices.clear();
    g_diLastState.clear();

    // Release DirectInput
    if (g_pDirectInput)
    {
        g_pDirectInput->Release();
        g_pDirectInput = nullptr;
    }

    g_bInitialized = false;
}

bool CheckGamepadActivity()
{
    if (!g_bInitialized)
    {
        InitGamepadTracking(nullptr);
    }

    // Periodically re-enumerate to detect hot-plugged controllers
    DWORD dwNow = GetTickCount();
    if (dwNow - g_dwLastEnumTime > ENUM_INTERVAL_MS)
    {
        EnumerateDirectInputDevices();
        g_dwLastEnumTime = dwNow;
    }

    bool bActivity = false;

    // Check XInput controllers (Xbox-style)
    bActivity |= CheckXInputActivity();

    // Check DirectInput controllers (PS5, generic, etc.)
    bActivity |= CheckDirectInputActivity();

    return bActivity;
}
