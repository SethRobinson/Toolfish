#pragma once

// Initialize gamepad tracking (call once at startup)
void InitGamepadTracking();

// Check if any gamepad button was pressed since last check
// Returns true if activity detected on any connected gamepad
bool CheckGamepadActivity();
