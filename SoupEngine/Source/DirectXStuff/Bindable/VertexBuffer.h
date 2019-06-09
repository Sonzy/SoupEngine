#pragma once
#include "Bindable.h"
#include "Error Handling/GraphicsErrorMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		:stride(sizeof(vertices))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = sizeof(V) * vertices.size());
		bufferDesc.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = vertices.data();

		GFX_THROW_INFO(device->CreateBuffer(&bufferDesc, &subData, &vertexBuffer));
	}


	void Bind(Graphics& gfx) noexcept override;


protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};