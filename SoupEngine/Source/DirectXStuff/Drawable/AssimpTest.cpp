#include "AssimpTest.h"
#include "BindableBase.h"
#include "Error Handling/GraphicsErrorMacros.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "DirectXStuff/3DSystems/Vertex.h"

AssimpTest::AssimpTest(Graphics & gfx, std::mt19937 & rng,
	std::uniform_real_distribution<float>& aDist,
	std::uniform_real_distribution<float>& dDist
	, std::uniform_real_distribution<float>& oDist,
	std::uniform_real_distribution<float>& rDist,
	DirectX::XMFLOAT3 material, float scale)
	: TestObject(gfx, rng, aDist, dDist, oDist, rDist)
{
	if (!IsStaticInitialised())
	{
		using VertexSys::VertexLayout;

		VertexSys::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));


		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("Source\\Models\\suzanne.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);
		const auto pMesh = pModel->mMeshes[0];

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				DirectX::XMFLOAT3{ pMesh->mVertices[i].x * scale, pMesh->mVertices[i].y * scale, pMesh->mVertices[i].z * scale },
				*reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddStaticBind(std::make_unique<Bind::VertexBuffer>(gfx, vbuf));


		auto vShader = std::make_unique<Bind::VertexShader>(gfx, L"Source/Shaders/PerPixelVS.cso");
		auto vShaderByteCode = vShader->GetByteCode();

		//Bind the pixel and vertex shaders
		AddStaticBind(std::move(vShader));
		AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"Source/Shaders/PerPixelPS.cso"));

		AddStaticIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, indices));

		AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, vbuf.GetLayout().GetD3DLayout(), vShaderByteCode));

		//Bind the topology
		AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} materialConstants;
		materialConstants.color = material;
		AddStaticBind(std::make_unique<Bind::PixelConstantBuffer<PSMaterialConstant>>(gfx, materialConstants, 1u));
	}
	else
		SetIndexFromStatic();

	//Bind the transform cbuffer
	AddBind(std::make_unique<Bind::TransformCBuffer>(gfx, *this));
}
