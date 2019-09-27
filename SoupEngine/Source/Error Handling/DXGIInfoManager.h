#pragma once
#include <vector>
#include <wrl.h>
#include <string>
#include <dxgidebug.h>

/* Get the dxerror info instead of having to look in output log */
class DXGIInfoManager
{
public:
	DXGIInfoManager();
	~DXGIInfoManager() = default;
	DXGIInfoManager(const DXGIInfoManager&) = delete;
	DXGIInfoManager& operator=(const DXGIInfoManager&) = delete;

	void Set() noexcept;
	std::vector<std::string> GetMessages() const;

private:
	unsigned long long next = 0;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiInfoQueue = nullptr;
};

