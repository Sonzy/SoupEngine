#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(Graphics & gfx, const Drawable & parent, UINT slot)
	: parent(parent), slot(slot)

{
	if (!vcBuffer)
		vcBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
}

void TransformCBuffer::Bind(Graphics& gfx) noexcept
{
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
	const Transforms tf = 
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	vcBuffer->Update(gfx, tf);
	vcBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::vcBuffer;
