#pragma once
#include "Drawable.h"
#include <random>

class Box : public Drawable
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& aDist,
		std::uniform_real_distribution<float>& dDist,
		std::uniform_real_distribution<float>& oDist,
		std::uniform_real_distribution<float>& rDist);

	void Update(float deltaTime) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	//Position stuff
	float r;
	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	//Speed
	float dRoll;
	float dPitch;
	float dYaw;

	//Rotation from centre of world
	float dTheta;
	float dPhi;
	float dChi;
};
