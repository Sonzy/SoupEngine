#include "Camera.h"
#include "ImGui/imgui_impl_dx11.h"
#include "DirectXStuff/Drawable/SoupMath.h"
#include <DirectXMath.h>

Camera::Camera()
{
	Reset();
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	const DirectX::XMVECTOR forwardBaseVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//Apply rotations to the base vector
	const auto lookVector = DirectX::XMVector3Transform(forwardBaseVector,
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f)
	);


	// Generate a camera transform
	DirectX::XMFLOAT3 fLookVector;
	DirectX::XMFLOAT3 camTarget;

	DirectX::XMStoreFloat3(&fLookVector, lookVector);
	camTarget.x = position.x + fLookVector.x;
	camTarget.y = position.y + fLookVector.y;
	camTarget.z = position.z + fLookVector.z;

	DirectX::XMVECTOR vCamTarget = DirectX::XMLoadFloat3(&camTarget);
	DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&position);
	return DirectX::XMMatrixLookAtLH(camPosition, vCamTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &position.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Y", &position.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Z", &position.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Rotation");
		ImGui::SliderAngle("Pitch", &pitch, 0.995f * -90.0f , 0.995f * 90.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	position = { 0.0f, 7.0f, -18.0f };

	pitch = 0.0f;
	yaw = 0.0f;
}

void Camera::Translate(DirectX::XMFLOAT3 translation)
{
	DirectX::XMStoreFloat3( &translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
		DirectX::XMMatrixScaling(moveSpeed, moveSpeed, moveSpeed)
	));

	position =
	{
		position.x + translation.x,
		position.y + translation.y,
		position.z + translation.z
	};
}

void Camera::Rotate(float dx, float dy)
{
	yaw = wrap_angle( yaw + dx * rotationSpeed );
	pitch = std::clamp(pitch + dy * rotationSpeed, .995f * -PI / 2.0f, .995f * PI / 2.0f);
}
