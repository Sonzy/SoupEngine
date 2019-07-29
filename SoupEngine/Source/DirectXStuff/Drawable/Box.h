#pragma once
#include "TestObject.h"

class Box : public TestObject<Box>
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& aDist,
		std::uniform_real_distribution<float>& dDist,
		std::uniform_real_distribution<float>& oDist,
		std::uniform_real_distribution<float>& rDist,
		std::uniform_real_distribution<float>& bDist,
		DirectX::XMFLOAT3 materialColor);

	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	DirectX::XMFLOAT3X3 mTransform;
};
