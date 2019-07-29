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
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;

private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos; //pad to 16bytes
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};

private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};