#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
}