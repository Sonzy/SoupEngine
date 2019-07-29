#include "Box.h"

#include "BindableBase.h"
#include "Cube.h"

Box::Box(Graphics & gfx, std::mt19937 & rng,
	std::uniform_real_distribution<float>& aDist,
	std::uniform_real_distribution<float>& dDist,
	std::uniform_real_distribution<float>& oDist,
	std::uniform_real_distribution<float>& rDist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 materialColor)
	: TestObject(gfx, rng, aDist, dDist, oDist, rDist)
{
	if (!IsStaticInitialised())
	{
		//Create all the bindables
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 n;
		};

		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));


		auto vShader = std::make_unique<VertexShader>(gfx, L"Source/Shaders/PerPixelVS.cso");
		auto vShaderByteCode = vShader->GetByteCode();

		//Bind the pixel and vertex shaders
		AddStaticBind(std::move(vShader));
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"Source/Shaders/PerPixelPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		//Bind the input layout description
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, vShaderByteCode));

		//Bind the topology
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	}
	else
		SetIndexFromStatic();

	//Bind material colour
	struct PSMaterialConstantBuffer
	{
		DirectX::XMFLOAT3 color;
		float specularIntentsity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} colorCB;
	colorCB.color = materialColor;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstantBuffer>>(gfx, colorCB, 1));

	//Bind the transform cbuffer
	AddBind(std::make_unique<TransformCBuffer>(gfx, *this));

	DirectX::XMStoreFloat3x3(&mTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&mTransform) * TestObject::GetTransformXM();
}
