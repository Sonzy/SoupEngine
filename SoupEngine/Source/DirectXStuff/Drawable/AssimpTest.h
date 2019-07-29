#pragma once
#include "TestObject.h"
#include "DirectXStuff/Bindable/ConstantBuffer.h"

class AssimpTest : public TestObject<AssimpTest>
{
public:
	AssimpTest(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& aDist,
		std::uniform_real_distribution<float>& dDist,
		std::uniform_real_distribution<float>& oDist,
		std::uniform_real_distribution<float>& rDist,
		DirectX::XMFLOAT3 material, float scale);
};