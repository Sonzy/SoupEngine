#pragma once
#include "Error Handling/Exceptions.h"

#define GFX_EXCEPT_NOINFO(hr) HrException(__LINE__, __FILE__, hr)
#define GFX_THROW_NOINFO(hrCall) if(FAILED(hr = hrCall)) throw HrException(__LINE__, __FILE__, hr);

#ifndef NDEBUG
#define GFX_EXCEPT(hr) HrException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw InfoException(__LINE__, __FILE__, v); }}
#else
#define GFX_EXCEPT(hr) HrException(__LINE__, __FILE__, hr);
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) DeviceRemovedException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

// If we are in debug mode, we import infomanager into local scope
#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; DXGIInfoManager& infoManager = GetInfoManager((gfx))
#endif	