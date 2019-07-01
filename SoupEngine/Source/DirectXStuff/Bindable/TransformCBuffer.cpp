#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(Graphics & gfx, const Drawable & parent)
	: parent(parent)

{
	if (!vcBuffer)
		vcBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
}

void TransformCBuffer::Bind(Graphics& gfx) noexcept
{
	vcBuffer->Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
	vcBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuffer::vcBuffer;
