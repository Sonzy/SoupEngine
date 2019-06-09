#pragma once
#include "Window/WindowsMacroUndefs.h"
#include "Error Handling/DXGIInfoManager.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>


class Graphics
{
	friend class Bindable;
public:
	
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawIndexed(UINT count);

	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::FXMMATRIX GetProjection() const noexcept;

private:

#ifndef NDEBUG
	DXGIInfoManager infoManager;
#endif 

	DirectX::FXMMATRIX projection;

	// Used to create and allocate stuff
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	// Swap chain (front and back render buffer stoof)
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain = nullptr;
	// Device context (the stuff that we actually use to render)
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
	// Render target (canvas)
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTarget = nullptr;
	// Depth Stencil View
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
};

