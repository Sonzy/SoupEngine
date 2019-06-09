#include "IndexBuffer.h"
#include "Error Handling/GraphicsErrorMacros.h"

IndexBuffer::IndexBuffer(Graphics & gfx, const std::vector<unsigned short>& indices)
	: count ((UINT) indices.size())
{
	INFOMAN(gfx);

	//Bind the index buffer
	D3D11_BUFFER_DESC indexbufferDesc = {};
	indexbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexbufferDesc.CPUAccessFlags = 0u;
	indexbufferDesc.MiscFlags = 0u;
	indexbufferDesc.ByteWidth = sizeof(indices);
	indexbufferDesc.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA indexSubData = {};
	indexSubData.pSysMem = indices.data();
	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&indexbufferDesc, &indexSubData, &indexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
