#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

//Can use this to automatically set up linker settings
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_THROW_NOINFO(hrCall) if(FAILED(hr = hrCall)) throw Graphics::HrException(__LINE__, __FILE__, hr);

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Graphics::InfoException(__LINE__, __FILE__, v); }}
#else
#define GFX_EXCEPT(hr) Grahpics::HrException(__LINE__, __FILE__, hr);
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, hr)
#define GFX_THROW_INFO_ONLY(call) (call)
#endif


Graphics::Graphics(HWND hWnd)
{
	// USed to check D3D function stuff
	HRESULT hr;

	//Create swapchain options
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
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
}

void Graphics::DrawTestTriangle(float angle)
{
	//Create a array of vertexes to make 2D triangle
	Vertex2D vertices[] = 
	{	
		{ 0.0f,0.5f, 255, 0, 0 },
		{ 0.5f, -0.5f , 0, 255, 0},
		{ -0.5f, -0.5f,  0, 0, 255 },
		{ -0.3f,0.3f, 255, 0, 255 },
		{ 0.3f,0.3f, 255, 0, 0 },
		{ 0.0f, -0.8f, 255, 255, 0 },
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
	bufferDesc.StructureByteStride = sizeof(Vertex2D);

	D3D11_SUBRESOURCE_DATA subData = {};
	subData.pSysMem = vertices;

	HRESULT hr;
	GFX_THROW_INFO(device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer));


	//Bind vertex buffer to the pipeline
	const UINT stride = sizeof(Vertex2D);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);


	//Create index buffer (Index drawing, avoid drawing multiple triangles when unneccesary
	const unsigned short indices[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		2,1,5,
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
		struct
		{
			float element[4][4];
		} transformation;
	};

	//CPU is row major, GPU is column major. Remember row_major specifier in vertex shader if we do it this way
	const ConstantBuffer cb = 
	{
		{	
			(3.0f / 4.0f) * std::cos(angle),	std::sin(angle),	0.0f,				0.0f,  //Scale matrix by our current aspect ratio
			(3.0f / 4.0f) * -std::sin(angle),	std::cos(angle),	0.0f,				0.0f,
			0.0f,				0.0f,				1.0f,				0.0f,
			0.0f,				0.0f,				0.0f,				1.0f,
		}
	};

	// Create the buffer
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
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


	wrl::ComPtr<ID3DBlob> blob;
	//Create a pixel shader
	wrl::ComPtr<ID3D11PixelShader> pShader;
	GFX_THROW_INFO(D3DReadFileToBlob(L"BasicPShader.cso", &blob));
	GFX_THROW_INFO(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pShader));
	context->PSSetShader(pShader.Get(), nullptr, 0);


	//Create a vertex shader
	wrl::ComPtr<ID3D11VertexShader> vShader;

	GFX_THROW_INFO(D3DReadFileToBlob(L"BasicVShaderhlsl.cso", &blob));
	GFX_THROW_INFO(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vShader));
	context->VSSetShader(vShader.Get(), nullptr, 0);

	//Describe the input layout for the vertex shader
	wrl::ComPtr<ID3D11InputLayout> inputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0 } // offset by 8bytes since the color is 8 bytes from teh start of the ied
	};
	device->CreateInputLayout(ied, (UINT)std::size(ied), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
	context->IASetInputLayout(inputLayout.Get());


	//Bind Render target
	context->OMSetRenderTargets(1u, renderTarget.GetAddressOf(), nullptr);

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
