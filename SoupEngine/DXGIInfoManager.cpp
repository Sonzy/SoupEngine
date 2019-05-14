#include "DXGIInfoManager.h"
#include "Window.h"
#include "Graphics.h"
#include <memory>

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrCall) if(FAILED(hr = hrCall)) throw Graphics::HrException(__LINE__, __FILE__, hr);


DXGIInfoManager::DXGIInfoManager()
{
	// Define the function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// Load the DLL tht contains the function
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (!hModDxgiDebug)
		throw WIND_LAST_EXCEPT();

	//Get address of function in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
	if (!DxgiGetDebugInterface)
		throw WIND_LAST_EXCEPT();

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &dxgiInfoQueue));
}


void DXGIInfoManager::Set() noexcept
{
	//Set the index (next) so that the next call to getmessages() will only get errors after this call
	next = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DXGIInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;
		//Get the size of message i in bytes
		GFX_THROW_NOINFO(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		//Allocate memory for the message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto message = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		//Get the message and push its descript to the vector
		GFX_THROW_NOINFO(dxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, message, &messageLength));
		messages.emplace_back(message->pDescription);
	}

	return messages;
}
