#include "Bindable.h"

using namespace Bind;
ID3D11DeviceContext * Bindable::GetContext(Graphics & gfx) noexcept
{
	return gfx.context.Get();
}

ID3D11Device * Bindable::GetDevice(Graphics & gfx) noexcept
{
	return gfx.device.Get();
}

DXGIInfoManager & Bindable::GetInfoManager(Graphics & gfx) noexcept
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error("Tried to access gfx.infomanager in release config");
#endif // !NDEBUG
}
