#pragma once

#include "CGlobals.h"

//#include "targetver.h"
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#include <new>
#include <strsafe.h>
#include <objbase.h>
#pragma warning(push)
#pragma warning(disable : 4201)
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#pragma warning(pop)

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

#include <string>
void AdjustMic(std::string partialName, float micLevel);
