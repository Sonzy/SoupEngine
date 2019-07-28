#pragma once
#include "DirectXStuff/Graphics.h"
#include "DirectXStuff/Drawable/SolidSphere.h"
#include "DirectXStuff/Bindable/ConstantBuffer.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept;
	void Bind(Graphics& gfx) const noexcept;

private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding; //pad to 16byte alignment
	};

private:
	DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};