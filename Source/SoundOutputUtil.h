// SoundOutputUtil.h
// Utility functions for enumerating and setting sound output devices

#pragma once

#include <string>
#include <vector>

// Structure to hold device information
struct SoundOutputDevice
{
    std::wstring deviceId;      // Internal device ID for SetDefaultEndpoint
    std::wstring friendlyName;  // User-friendly device name
};

// Enumerate all active audio output devices
// Returns a vector of SoundOutputDevice structures
std::vector<SoundOutputDevice> EnumerateOutputDevices();

// Set the default audio output device by substring match
// partialName: A substring to match against device friendly names (case-insensitive)
// Returns true if a device was found and set, false otherwise
bool SetDefaultOutputDevice(const char* partialName);

// Get a formatted string of all available output devices (for logging)
std::string GetOutputDeviceListString();
