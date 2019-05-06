#pragma once
#include "WindowsMacroUndefs.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

private:

	// Direct X Device (Hardware I think)
	ID3D11Device* device = nullptr;
	// Swap chain (front and back render buffer stoof)
	IDXGISwapChain* swapChain = nullptr;
	// Device context (the stuff that we actually use to render)
	ID3D11DeviceContext* context = nullptr;
};

