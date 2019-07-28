#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "Error Handling/GraphicsErrorMacros.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

//Bindables
#include "Bindable/IndexBuffer.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//Can use this to automatically set up linker settings
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
	: projection(DirectX::FXMMATRIX())
{
	// USed to check D3D function stuff
	HRESULT hr;

	//Create swapchain options
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0;

#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 


	//Create the device, front/back buffers, swap chan and rendering context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		swapCreateFlags, nullptr, 0, D3D11_SDK_VERSION, &sd, swapChain.GetAddressOf(), device.GetAddressOf(),
		nullptr, context.GetAddressOf()));

	//Create a render target
	wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;


	GFX_THROW_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
	GFX_THROW_INFO(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget));

	//Setup Zbuffer

	//create depth stencil state

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = FALSE;

	wrl::ComPtr<ID3D11DepthStencilState> dsState;
	GFX_THROW_INFO(device->CreateDepthStencilState(&dsDesc, &dsState));

	//Bind depth state
	context->OMSetDepthStencilState(dsState.Get(), 1u);

	// Create the depth stencil texutre
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u; // Can create an array in 1 resource
	descDepth.Format = DXGI_FORMAT_D32_FLOAT; // D32 is special for depth. Uses 32bit floats

	descDepth.SampleDesc.Count = 1u; //Used for AA in the future
	descDepth.SampleDesc.Quality = 0u;

	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO(device->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	//Create the view of the depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(device->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &depthStencilView));

	//bind the depth stencil view to om
	context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), depthStencilView.Get());

	//Setup viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800.0f;
	vp.Height = 800.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	context->RSSetViewports(1u, &vp);

	ImGui_ImplDX11_Init(device.Get(), context.Get());
} 

void Graphics::EndFrame()
{
	if (GUIEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;

#ifndef NDEBUG
	infoManager.Set();
#endif // !NDEBUG

	if (FAILED(hr = swapChain->Present(1, 0)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(device->GetDeviceRemovedReason());
		}
		else
		{
			GFX_EXCEPT(hr);
		}
	}
}

void Graphics::BeginFrame(float r, float g, float b, float a)
{
	if (GUIEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { r, g, b, a };
	context->ClearRenderTargetView(renderTarget.Get(), color);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count)
{
	GFX_THROW_INFO_ONLY(context->DrawIndexed(count, 0u, 0u));
}

void Graphics::SetProjection(DirectX::FXMMATRIX projec) noexcept
{
	projection = projec;
}

DirectX::FXMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam) noexcept
{
	camera = cam;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept
{
	return camera;
}

void Graphics::EnableGUI() noexcept
{
	GUIEnabled = true;
}

void Graphics::DisableGUI() noexcept
{
	GUIEnabled = false;
}

bool Graphics::IsGUIEnabled() const noexcept
{
	return GUIEnabled;
}

//===========================================================
// Exceptions
//===========================================================


