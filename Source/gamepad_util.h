#pragma once

// Initialize gamepad tracking (call once at startup)
// Pass the application's HINSTANCE for DirectInput initialization
void InitGamepadTracking(HINSTANCE hInstance = nullptr);

// Shutdown and cleanup gamepad resources (call on app exit)
void ShutdownGamepadTracking();

// Check if any gamepad button was pressed since last check
// Returns true if activity detected on any connected gamepad
// Checks both XInput (Xbox) and DirectInput (PS5, generic) controllers
bool CheckGamepadActivity();
