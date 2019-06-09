#pragma once
#include "Bindable.h"
#include "Error Handling/GraphicsErrorMacros.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:

	void Update(Graphics& gfx, const C& consts)
	{
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
		//Map - locks it and allows you to write to memory
		GFX_THROW_INFO(GetContext()->Map(constantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));

		//memcpy - copies data from param 2 to param 1. (param 3 is size of data to copy)
		memcpy(msr.pData, &consts, sizeof(consts));

		//Unmap - to unlock to continue
		GetContext(gfx)->Unmap(constantBuffer.Get(), 0u);
	}

	ConstantBuffer(Graphics& gfx, const C& consts)
	{
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice()->CreateBuffer(&cbd, &csd, &constantBuffer));
	}

	ConstantBuffer(Graphics& gfx)
	{
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		GFX_THROW_INFO(GetDevice()->CreateBuffer(&cbd, nullptr, &constantBuffer));
	}


private:
	wrl::ComPtr<ID3D11Buffer> constantBuffer;
};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	//These allow you to access parent class stuff in a template child class by importing them
	//or could do this->GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	using ConstantBuffer<C>::constantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	}
};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	//These allow you to access parent class stuff in a template child class
	using ConstantBuffer<C>::constantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	}
};