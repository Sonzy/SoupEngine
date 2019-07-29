#pragma once
#include "DrawableBase.h"
#include <random>
#include "SoupMath.h"

template<class T>
class TestObject : public DrawableBase<T>
{
public:
	TestObject(Graphics& gfx, std::mt19937& rng,
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
	{}

	void Update(float deltaTime) noexcept
	{
		roll += wrap_angle(dRoll * deltaTime);
		pitch += wrap_angle(dPitch * deltaTime);
		yaw += wrap_angle(dYaw * deltaTime);
		theta += wrap_angle(dTheta * deltaTime);
		phi += wrap_angle(dPhi * deltaTime);
		chi += wrap_angle(dChi * deltaTime);
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * //Rotate around box centre
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) * //Move relative to origin
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi); //Rotate around world centre
	}

protected:
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