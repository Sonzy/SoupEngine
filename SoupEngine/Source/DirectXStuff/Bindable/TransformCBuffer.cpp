#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(Graphics & gfx, const Drawable & parent)
	: parent(parent)

{
	if (!vcBuffer)
		vcBuffer = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
}

void TransformCBuffer::Bind(Graphics& gfx) noexcept
{
	const auto model = parent.GetTransformXM();
	const Transforms tf = 
	{
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model *
			gfx.GetCamera() *
			gfx.GetProjection()
		)
	};
	vcBuffer->Update(gfx, tf);
	vcBuffer->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::Transforms>> TransformCBuffer::vcBuffer;
