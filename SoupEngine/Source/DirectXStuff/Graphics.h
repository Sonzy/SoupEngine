#pragma once
#include "Window/WindowsMacroUndefs.h"
#include "Error Handling/DXGIInfoManager.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include <memory>

namespace Bind
{
	class Bindable;
}

class Graphics
{
	friend class Bind::Bindable;
public:
	
	Graphics(HWND hWnd, int width, int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void BeginFrame(float r, float g, float b, float a);
	void DrawIndexed(UINT count);

	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::FXMMATRIX GetProjection() const noexcept;
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

	//GUI stuff
	void EnableGUI() noexcept;
	void DisableGUI() noexcept;

	bool IsGUIEnabled() const noexcept;

private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	bool GUIEnabled = true;

#ifndef NDEBUG
	DXGIInfoManager infoManager;
#endif 



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

