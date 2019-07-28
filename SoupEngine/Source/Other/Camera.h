#pragma once
#include "DirectXStuff/Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;

private:
	float r = 20.0f;
	float theta = 0.0f; //Position
	float phi = 0.0f;

	float pitch = 0.0f;
	float yaw = 0.0f; //Rotation
	float roll = 0.0f;
};