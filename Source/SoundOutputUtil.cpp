// SoundOutputUtil.cpp
// Implementation of sound output device enumeration and switching

#include "stdafx.h"
#include "SoundOutputUtil.h"
#include "IPolicyConfig.h"

#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <algorithm>
#include <cctype>

#include "..\Shared\all.h"
#include "..\Shared\uni.h"

// Helper function to convert wide string to lowercase for case-insensitive comparison
static std::wstring ToLowerW(const std::wstring& str)
{
    std::wstring result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::towlower);
    return result;
}

// Helper function to convert narrow string to wide string
static std::wstring ToWideString(const char* str)
{
    if (!str) return L"";
    int len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    if (len <= 0) return L"";
    std::wstring result(len - 1, L'\0');
    MultiByteToWideChar(CP_ACP, 0, str, -1, &result[0], len);
    return result;
}

// Helper function to convert wide string to narrow string
static std::string ToNarrowString(const std::wstring& wstr)
{
    if (wstr.empty()) return "";
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return "";
    std::string result(len - 1, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &result[0], len, NULL, NULL);
    return result;
}

std::vector<SoundOutputDevice> EnumerateOutputDevices()
{
    std::vector<SoundOutputDevice> devices;
    HRESULT hr;

    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDeviceCollection* pCollection = NULL;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr) || !pEnumerator)
    {
        return devices;
    }

    // Enumerate active audio render (output) endpoints
    hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pCollection);
    if (FAILED(hr) || !pCollection)
    {
        pEnumerator->Release();
        return devices;
    }

    UINT count = 0;
    pCollection->GetCount(&count);

    for (UINT i = 0; i < count; i++)
    {
        IMMDevice* pDevice = NULL;
        hr = pCollection->Item(i, &pDevice);
        if (FAILED(hr) || !pDevice) continue;

        // Get device ID
        LPWSTR deviceId = NULL;
        hr = pDevice->GetId(&deviceId);
        if (FAILED(hr) || !deviceId)
        {
            pDevice->Release();
            continue;
        }

        // Get friendly name
        IPropertyStore* pProps = NULL;
        hr = pDevice->OpenPropertyStore(STGM_READ, &pProps);
        if (FAILED(hr) || !pProps)
        {
            CoTaskMemFree(deviceId);
            pDevice->Release();
            continue;
        }

        PROPVARIANT varName;
        PropVariantInit(&varName);
        hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);

        SoundOutputDevice device;
        device.deviceId = deviceId;
        
        if (SUCCEEDED(hr) && varName.vt == VT_LPWSTR && varName.pwszVal)
        {
            device.friendlyName = varName.pwszVal;
        }
        else
        {
            device.friendlyName = L"Unknown Device";
        }

        devices.push_back(device);

        PropVariantClear(&varName);
        pProps->Release();
        CoTaskMemFree(deviceId);
        pDevice->Release();
    }

    pCollection->Release();
    pEnumerator->Release();

    return devices;
}

std::string GetOutputDeviceListString()
{
    std::vector<SoundOutputDevice> devices = EnumerateOutputDevices();
    std::string result;

    for (size_t i = 0; i < devices.size(); i++)
    {
        if (i > 0) result += "\r\n";
        result += "  - " + ToNarrowString(devices[i].friendlyName);
    }

    if (result.empty())
    {
        result = "  (No active output devices found)";
    }

    return result;
}

bool SetDefaultOutputDevice(const char* partialName)
{
    if (!partialName || partialName[0] == '\0')
    {
        LogMsg(_T("Sound output: No device name specified."));
        return false;
    }

    // Initialize COM for this thread (we're running on a background thread)
    CoInitialize(NULL);

    std::vector<SoundOutputDevice> devices = EnumerateOutputDevices();
    
    if (devices.empty())
    {
        LogMsg(_T("Sound output: No active output devices found."));
        CoUninitialize();
        return false;
    }

    // Convert search string to lowercase wide string for case-insensitive comparison
    std::wstring searchLower = ToLowerW(ToWideString(partialName));
    
    // Find matching devices
    std::vector<size_t> matchingIndices;
    for (size_t i = 0; i < devices.size(); i++)
    {
        std::wstring nameLower = ToLowerW(devices[i].friendlyName);
        if (nameLower.find(searchLower) != std::wstring::npos)
        {
            matchingIndices.push_back(i);
        }
    }

    if (matchingIndices.empty())
    {
        // Use uni class to convert for logging
        uni searchName(partialName);
        LogMsg(_T("WARNING: No sound output device matches '%s'."), searchName.GetAuto());
        
        // Log available devices
        std::string deviceList = GetOutputDeviceListString();
        uni deviceListUni(deviceList.c_str());
        LogMsg(_T("Available devices:\r\n%s"), deviceListUni.GetAuto());
        CoUninitialize();
        return false;
    }

    // Use the first match
    size_t selectedIndex = matchingIndices[0];
    const SoundOutputDevice& selectedDevice = devices[selectedIndex];

    // Log what we're doing - use wide string directly since friendlyName is already wide
    if (matchingIndices.size() > 1)
    {
        LogMsg(_T("Sound output set to: %s (first of %d matches)"), 
            selectedDevice.friendlyName.c_str(), (int)matchingIndices.size());
    }
    else
    {
        LogMsg(_T("Sound output set to: %s"), selectedDevice.friendlyName.c_str());
    }

    // Use IPolicyConfig to set the default endpoint
    IPolicyConfig* pPolicyConfig = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigClient), NULL, CLSCTX_ALL,
        __uuidof(IPolicyConfig), (void**)&pPolicyConfig);

    if (FAILED(hr) || !pPolicyConfig)
    {
        LogMsg(_T("Unable to change sound output device - this feature may not be supported on your version of Windows."));
        CoUninitialize();
        return false;
    }

    // Set as default for all roles (Console, Multimedia, Communications)
    hr = pPolicyConfig->SetDefaultEndpoint(selectedDevice.deviceId.c_str(), eConsole);
    if (SUCCEEDED(hr))
    {
        pPolicyConfig->SetDefaultEndpoint(selectedDevice.deviceId.c_str(), eMultimedia);
        pPolicyConfig->SetDefaultEndpoint(selectedDevice.deviceId.c_str(), eCommunications);
    }

    pPolicyConfig->Release();

    if (FAILED(hr))
    {
        LogMsg(_T("Failed to set sound output device (error: 0x%08X)."), hr);
        CoUninitialize();
        return false;
    }

    CoUninitialize();
    return true;
}
