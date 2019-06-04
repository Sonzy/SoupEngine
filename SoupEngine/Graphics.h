#pragma once
#include "WindowsMacroUndefs.h"
#include <d3d11.h>
#include "SoupException.h"
#include <vector>
#include <wrl.h>
#include "DXGIInfoManager.h"

struct Vertex2D
{
	struct
	{
		float x;
		float y;
	} pos;

	
	//colors (specify as single bytes)
	struct 
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} color;
};

class Graphics
{
public:
	class Exception : public SoupException
	{
		using SoupException::SoupException;
	};

	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> extrainfo = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};


	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMessages) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};

	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;

	void DrawTestTriangle(float angle);
private:

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
};

