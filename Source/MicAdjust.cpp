#include "stdafx.h"
#include "MicAdjust.h"

#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <Endpointvolume.h>

using namespace std;

//
//  Retrieves the device friendly name for a particular device in a device collection.  
//
//  The returned string was allocated using malloc() so it should be freed using free();
//
LPWSTR GetDeviceName(IMMDeviceCollection *DeviceCollection, UINT DeviceIndex)
{
	IMMDevice *device;
	LPWSTR deviceId;
	HRESULT hr;

	hr = DeviceCollection->Item(DeviceIndex, &device);
	if (FAILED(hr))
	{
		printf("Unable to get device %d: %x\n", DeviceIndex, hr);
		return NULL;
	}
	hr = device->GetId(&deviceId);
	if (FAILED(hr))
	{
		printf("Unable to get device %d id: %x\n", DeviceIndex, hr);
		return NULL;
	}

	IPropertyStore *propertyStore;
	hr = device->OpenPropertyStore(STGM_READ, &propertyStore);
	SafeRelease(&device);
	if (FAILED(hr))
	{
		printf("Unable to open device %d property store: %x\n", DeviceIndex, hr);
		return NULL;
	}

	PROPVARIANT friendlyName;
	PropVariantInit(&friendlyName);
	hr = propertyStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
	SafeRelease(&propertyStore);

	if (FAILED(hr))
	{
		printf("Unable to retrieve friendly name for device %d : %x\n", DeviceIndex, hr);
		return NULL;
	}

	wchar_t deviceName[128];
	hr = StringCbPrintf(deviceName, sizeof(deviceName), L"%s (%s)", friendlyName.vt != VT_LPWSTR ? L"Unknown" : friendlyName.pwszVal, deviceId);
	if (FAILED(hr))
	{
		printf("Unable to format friendly name for device %d : %x\n", DeviceIndex, hr);
		return NULL;
	}

	PropVariantClear(&friendlyName);
	CoTaskMemFree(deviceId);

	wchar_t *returnValue = _wcsdup(deviceName);
	if (returnValue == NULL)
	{
		printf("Unable to allocate buffer for return\n");
		return NULL;
	}
	return returnValue;
}


void AdjustMic(string partialName, float micLevel)
{
	//we're in a thread now, so need to do this

	CoInitialize(NULL);
	int devicesAdjusted = 0;
	string deviceNames;
	HRESULT hr;

	IMMDeviceEnumerator *deviceEnumerator = NULL;
	IMMDeviceCollection *deviceCollection = NULL;

	IMMDevice *device = NULL;

	//We initialize the device enumerator here
	hr = CoCreateInstance( __uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, 
		IID_PPV_ARGS(&deviceEnumerator) );
	if (FAILED(hr))
	{
		LogMsg("Unable to instantiate device enumerator: %x\n", hr);
		goto Exit;
	}

	
	//Here we enumerate the audio endpoints of interest (in this case audio capture endpoints)
	//into our device collection. We use "eCapture"
	//for audio capture endpoints, "eRender" for 
	//audio output endpoints and "eAll" for all audio endpoints 
	hr = deviceEnumerator->EnumAudioEndpoints( eCapture, DEVICE_STATE_ACTIVE, 
		&deviceCollection );
	if (FAILED(hr))
	{
		LogMsg("Mic volume adjust: Unable to retrieve device collection: %x\n", hr);
		goto Exit;
	}

	UINT deviceCount;

	hr = deviceCollection->GetCount(&deviceCount);
	if (FAILED(hr))
	{
		printf("Unable to get device collection length: %x\n", hr);
		goto Exit;
	}


	//
	//This loop goes over each audio endpoint in our device collection,
	//gets and diplays its friendly name and then tries to mute it
	//


	for (UINT i = 0 ; i < deviceCount ; i += 1)
	{
		LPWSTR deviceName;

		//Here we use the GetDeviceName() function provided with the sample 
		//(see source code zip)
		deviceName = GetDeviceName(deviceCollection, i); //Get device friendly name

		uni devName(deviceName);


		if (string(devName.to_st()).find(partialName) != string::npos)
		{
			devicesAdjusted++;
			//yep
		} else
		{
			deviceNames += string(devName.to_st())+"\r\n";
			//nope
			continue;
		}


		if (deviceName == NULL) goto Exit;

		//printf("Device to be muted has index: %d and name: %S\n", i, deviceName);

		//this needs to be done because name is stored in a heap allocated buffer
		free(deviceName);

		device = NULL;

		//Put device ref into device var
		hr = deviceCollection->Item(i, &device);
		if (FAILED(hr))
		{
			printf("Unable to retrieve device %d: %x\n", i, hr);
			goto Exit;
		}

		//This is the Core Audio interface of interest
		IAudioEndpointVolume *endpointVolume = NULL;

		//We activate it here
		hr = device->Activate( __uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, 
			reinterpret_cast<void **>(&endpointVolume) );
		if (FAILED(hr))
		{
			printf("Unable to activate endpoint volume on output device: %x\n", hr);
			goto Exit;
		}

		//to mute

		/*
		hr = endpointVolume->SetMute(TRUE, NULL); //Try to mute endpoint here
		if (FAILED(hr))
		{
		printf("Unable to set mute state on endpoint: %x\n", hr);
		goto Exit;
		}
		else
		printf("Endpoint muted successfully!\n");
		*/

		{

			float masterVolScaler;
			endpointVolume->GetMasterVolumeLevelScalar(&masterVolScaler);
			if (masterVolScaler != micLevel)
			{
				LogMsg("Changed volume to %.2f percent on device %s, it WAS at %.2f.", micLevel, devName.to_st(), masterVolScaler);
				endpointVolume->SetMasterVolumeLevelScalar(micLevel, 0);

			}

		}
	}

	if (devicesAdjusted == 0)
	{
		LogMsg("Couldn't find any devices with %s in the name to adjust.", partialName.c_str());
		LogMsg("Devices found:\r\n%s",deviceNames.c_str());
	}


Exit: //Core Audio and COM clean up here
	SafeRelease(&deviceCollection);
	SafeRelease(&deviceEnumerator);
	SafeRelease(&device);
	 CoUninitialize();

}
