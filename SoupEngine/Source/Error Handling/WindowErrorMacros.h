#pragma once


// Macro to quickly throw exception
#define WIND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define WIND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())
#define WIND_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, __FILE__)