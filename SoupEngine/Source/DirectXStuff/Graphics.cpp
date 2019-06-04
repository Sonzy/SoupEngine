#include "Graphics.h"
#include "Error Handling/dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "Error Handling/GraphicsErrorMacros.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//Can use this to automatically set up linker settings
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
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
} 

void Graphics::EndFrame()
{
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

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = {red, green, blue, 1.0f};
	context->ClearRenderTargetView(renderTarget.Get(), color);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestTriangle(float angle, float x, float z)
{
	//Create a array of vertexes to make 2D triangle
	//Vertex2D vertices[] = 
	//{	
	//	{ 0.0f,0.5f, 255, 0, 0 },
	//	{ 0.5f, -0.5f , 0, 255, 0},
	//	{ -0.5f, -0.5f,  0, 0, 255 },
	//	{ -0.3f,0.3f, 255, 0, 255 },
	//	{ 0.3f,0.3f, 255, 0, 0 },
	//	{ 0.0f, -0.8f, 255, 255, 0 },
	//};

	// Create an array of vertieces to make a cube
	Vertex vertices[] =
	{
		{-1.0f, -1.0f, -1.0f },
		{1.0f, -1.0f, -1.0f	 },
		{-1.0f, 1.0f, -1.0f	 },
		{1.0f, 1.0f, -1.0f,	 },
		{-1.0f, -1.0f, 1.0f	 },
		{1.0f, -1.0f, 1.0f	 },
		{-1.0f, 1.0f, 1.0f	 },
		{ 1.0f, 1.0f, 1.0f	 },
	};

	//vertices[0].color.g = 255;

	//Create a buffer for the vertices
	wrl::ComPtr<ID3D11Buffer> vertexBuffer;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = vertices;

	HRESULT hr;
	GFX_THROW_INFO(device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer));


	//Bind vertex buffer to the pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);


	//Create index buffer for cube (remember winding direction)
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC indexbufferDesc = {};
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.CPUAccessFlags = 0u;
	indexbufferDesc.MiscFlags = 0u;
	indexbufferDesc.ByteWidth = sizeof(indices);
	indexbufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSubData = {};
	indexSubData.pSysMem = indices;
	GFX_THROW_INFO(device->CreateBuffer(&indexbufferDesc, &indexSubData, &pIndexBuffer));

	context->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// Create a const buffer for our transformation matrices
	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};

	//CPU is row major, GPU is column major. Use transpose to convert to column major
	const ConstantBuffer cb =
	{
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixRotationX(angle) *
			dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
			dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f))
	};

	// Create the buffer
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_INFO(device->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	//bind the const buffer to the vertex shader
	context->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


	//Create a constant buffer to be able to give each triangle IDs to use to look up the colours to specify for each one
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a; //Not using alpha atm but want to keep this struct 16 bit aligned
		} face_colours[6]; //store each colour for the cube faces
	};

	//Define the buffer
	const ConstantBuffer2 cb2 = 
	{
		{
			{1.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f },
			{0.0f, 1.0f, 0.0f, 1.0f },
			{0.0f, 0.0f, 1.0f, 1.0f },
			{1.0f, 1.0f, 0.0f, 1.0f },
			{0.0f, 1.0f, 1.0f, 1.0f },
		}
	};

	// Bind to the PS
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_INFO(device->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

	//bind the const buffer to the pixel shader
	context->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());


	wrl::ComPtr<ID3DBlob> blob;
	//Create a pixel shader
	wrl::ComPtr<ID3D11PixelShader> pShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"Source/Shaders/BasicPShader.cso", &blob));
	GFX_THROW_INFO(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pShader));
	context->PSSetShader(pShader.Get(), nullptr, 0);


	//Create a vertex shader
	wrl::ComPtr<ID3D11VertexShader> vShader;

	GFX_THROW_INFO(D3DReadFileToBlob(L"Source/Shaders/BasicVShaderhlsl.cso", &blob));
	GFX_THROW_INFO(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vShader));
	context->VSSetShader(vShader.Get(), nullptr, 0);

	//Describe the input layout for the vertex shader
	wrl::ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	context->IASetInputLayout(inputLayout.Get());

	//Setup topology to triangle list
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Setup viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1u, &vp);

	//DRAW HUEHEUHE
	//GFX_THROW_INFO_ONLY(context->Draw(std::size(vertices), 0u));

	//Draw using indeces
	GFX_THROW_INFO_ONLY(context->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}


//===========================================================
// Exceptions
//===========================================================

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " ( " << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "Error String: " << GetErrorString() << std::endl
		<< "Description: " << GetErrorDescription() << std::endl;
	if (!info.empty())
		oss << "\nError info: " << GetErrorInfo() << std::endl << std::endl;

	oss << GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Soup Graphics Exception";
}

Graphics::HrException::HrException(int line, const char * file, HRESULT hr, std::vector<std::string> extrainfo) noexcept
	: Exception(line, file), hr(hr)
{
	//Join all messages into a single string (may have newlines)
	for (const auto& m : extrainfo)
	{
		info += m;
		info.push_back('\n');

	}
	//Remove final new line if it exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Soup Graphics - Device removed";
}

Graphics::InfoException::InfoException(int line, const char * file, std::vector<std::string> infoMessages) noexcept
	: Exception(line, file)
{
	//Join all messages with newlines into a single string
	for (const auto &m : infoMessages)
	{
		info += m;
		info.push_back('\n');
	}

	//Remove the last new line if it exists
	if (!info.empty())
		info.pop_back();
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\nErrorInfo: " << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Soup Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}
