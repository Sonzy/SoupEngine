#include "Box.h"
#include "DirectXStuff/Bindable/VertexBuffer.h"
#include "DirectXStuff/Bindable/VertexShader.h"
#include "DirectXStuff/Bindable/PixelShader.h"
#include "DirectXStuff/Bindable/ConstantBuffer.h"
#include "DirectXStuff/Bindable/InputLayout.h"
#include "DirectXStuff/Bindable/Topology.h"
#include "DirectXStuff/Bindable/TransformCBuffer.h"

Box::Box(Graphics & gfx, std::mt19937 & rng,
	std::uniform_real_distribution<float>& aDist,
	std::uniform_real_distribution<float>& dDist,
	std::uniform_real_distribution<float>& oDist,
	std::uniform_real_distribution<float>& rDist)
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
	//Create all the bindables
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};

	//Create vertices for box mesh
	const std::vector<Vertex> vertices = 
	{
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
	};
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));


	auto vShader = std::make_unique<VertexShader>(gfx, L"Source/Shaders/BasicVShader.cso");
	auto vShaderByteCode = vShader->GetByteCode();
	
	//Bind the pixel and vertex shaders
	AddBind(std::move(vShader));
	AddBind(std::make_unique<PixelShader>(gfx, L"Source/Shaders/BasicPShader"));

	//Create and add index buffer
	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

	//Create a constant buffer to hold the colors
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};

	const ConstantBuffer2 cb2 = 
	{
		{
			{1.0f, 0.0f, 1.0f},
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 1.0f },
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

	//Bind the input layout description
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, vShaderByteCode));

	//Bind the topology
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	//Bind the transform cbuffer
	AddBind(std::make_unique<TransformCBuffer>(gfx, this*));
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
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * //Rotate around box centre
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) * //Move relative to origin
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) * //Rotate around world centre
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f); //Move relative to camera
}
