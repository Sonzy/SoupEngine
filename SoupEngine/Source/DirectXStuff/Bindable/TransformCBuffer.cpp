#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(Graphics & gfx, const Drawable & parent)
	: vcBuffer(gfx), parent(parent)

{
}

void TransformCBuffer::Bind(Graphics& gfx) noexcept
{
	vcBuffer.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
	vcBuffer.Bind(gfx);
}
