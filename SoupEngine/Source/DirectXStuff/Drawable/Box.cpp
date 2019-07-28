#include "Box.h"

#include "BindableBase.h"
#include "Cube.h"

Box::Box(Graphics & gfx, std::mt19937 & rng,
	std::uniform_real_distribution<float>& aDist,
	std::uniform_real_distribution<float>& dDist,
	std::uniform_real_distribution<float>& oDist,
	std::uniform_real_distribution<float>& rDist,
	std::uniform_real_distribution<float>& bdist)
	:
	r(rDist(rng)),
	dRoll(dDist(rng)),
	dPitch(dDist(rng)),
	dYaw(dDist(rng)),
	dPhi(oDist(rng)),
	dTheta(oDist(rng)),
	dChi(oDist(rng)),
	chi(aDist(rng)),
	theta(aDist(rng)),
	phi(aDist(rng))
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


	//Bind the transform cbuffer
	AddBind(std::make_unique<TransformCBuffer>(gfx, *this));

	DirectX::XMStoreFloat3x3(&mTransform, DirectX::XMMatrixScaling(1.0f, 1.0f, bdist(rng)));
}

void Box::Update(float deltaTime) noexcept
{
	roll += dRoll * deltaTime;
	pitch += dPitch * deltaTime;
	yaw += dYaw * deltaTime;
	theta += dTheta * deltaTime;
	phi += dPhi * deltaTime;
	chi += dChi * deltaTime;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMLoadFloat3x3(&mTransform) *
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * //Rotate around box centre
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) * //Move relative to origin
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) * //Rotate around world centre
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f); //Move relative to camera
}
