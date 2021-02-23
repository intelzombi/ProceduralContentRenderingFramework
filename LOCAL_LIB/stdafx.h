
#pragma once


#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#include <Windows.h>
#include <tchar.h>

#include <D3D10_1.h>
#include <D3D10.h>
#include <D3DX10.h>
#include <D3D11.h>
#include <D3DX11.h>
