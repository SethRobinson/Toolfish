#include "stdafx.h"
#include "gamepad_util.h"
#include <xinput.h>

// Maximum number of XInput controllers
#define XUSER_MAX_COUNT 4

// Track previous state for each controller
static DWORD g_dwLastPacketNumber[XUSER_MAX_COUNT] = {0, 0, 0, 0};
static WORD g_wLastButtons[XUSER_MAX_COUNT] = {0, 0, 0, 0};
static bool g_bInitialized = false;

// Mask for all digital buttons we care about (excludes analog sticks/triggers)
// This includes: D-pad, Start, Back, thumbstick clicks, A, B, X, Y, LB (left shoulder), RB (right shoulder)
static const WORD BUTTON_MASK = 
    XINPUT_GAMEPAD_DPAD_UP |
    XINPUT_GAMEPAD_DPAD_DOWN |
    XINPUT_GAMEPAD_DPAD_LEFT |
    XINPUT_GAMEPAD_DPAD_RIGHT |
    XINPUT_GAMEPAD_START |
    XINPUT_GAMEPAD_BACK |
    XINPUT_GAMEPAD_LEFT_THUMB |      // Left stick click
    XINPUT_GAMEPAD_RIGHT_THUMB |     // Right stick click
    XINPUT_GAMEPAD_LEFT_SHOULDER |   // LB button
    XINPUT_GAMEPAD_RIGHT_SHOULDER |  // RB button
    XINPUT_GAMEPAD_A |
    XINPUT_GAMEPAD_B |
    XINPUT_GAMEPAD_X |
    XINPUT_GAMEPAD_Y;

void InitGamepadTracking()
{
    // Initialize tracking state for all possible controllers
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        g_dwLastPacketNumber[i] = 0;
        g_wLastButtons[i] = 0;
    }
    g_bInitialized = true;
}

bool CheckGamepadActivity()
{
    if (!g_bInitialized)
    {
        InitGamepadTracking();
    }

    bool bActivityDetected = false;

    // Check all 4 possible controller slots
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        DWORD dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS)
        {
            // Controller is connected
            // Get current button state (masked to only the buttons we care about)
            WORD wCurrentButtons = state.Gamepad.wButtons & BUTTON_MASK;

            // Detect activity if button state changed (button was pressed OR released)
            // This catches quick taps even if button is released before next poll
            if (wCurrentButtons != g_wLastButtons[i])
            {
                // Button state changed - this is activity
                // (either a button was pressed or released)
                bActivityDetected = true;
            }

            // Update tracking state
            g_dwLastPacketNumber[i] = state.dwPacketNumber;
            g_wLastButtons[i] = wCurrentButtons;
        }
        else
        {
            // Controller disconnected - reset its tracking state
            g_dwLastPacketNumber[i] = 0;
            g_wLastButtons[i] = 0;
        }
    }

    return bActivityDetected;
}
