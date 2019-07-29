#pragma once
#include "ConstantBuffer.h"
#include "DirectXStuff/Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformCBuffer : public Bindable
{
private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX modelView;
	};

public:
	TransformCBuffer(Graphics& gfx, const Drawable& parent, UINT slot = 0);
	void Bind(Graphics& gfx) noexcept override;

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> vcBuffer;
	const Drawable& parent;
	UINT slot;


};