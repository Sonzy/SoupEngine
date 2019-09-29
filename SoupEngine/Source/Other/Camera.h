#pragma once
#include "DirectXStuff/Graphics.h"

class Camera
{
public:
	Camera();
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Translate(DirectX::XMFLOAT3 translation);
	void Rotate(float dx, float dy);

private:

	DirectX::XMFLOAT3 position;
	float pitch = 0.0f;
	float yaw = 0.0f;

	static constexpr float moveSpeed = 12.0f;
	static constexpr float rotationSpeed = 0.004f;
};