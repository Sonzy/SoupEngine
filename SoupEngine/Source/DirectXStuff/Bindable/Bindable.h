#pragma once
#include "DirectXStuff/Graphics.h"

namespace Bind
{
	class Bindable
	{
	public:
		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual ~Bindable() = default;
	protected:
		static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
		static DXGIInfoManager& GetInfoManager(Graphics& gfx) noexcept;
	};
}