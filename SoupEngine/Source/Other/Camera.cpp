#include "Camera.h"
#include "ImGui/imgui_impl_dx11.h"

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	namespace dx = DirectX;

	//Initialise camera position
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);

	//Initialise camera location
	return dx::XMMatrixLookAtLH(pos, dx::XMVectorZero(), dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) //Look at the centre
		* dx::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll); //rotate the camera
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 1.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Rotation");
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	r = 20.0f;
	theta = 0.0f; //Position
	phi = 0.0f;

	pitch = 0.0f;
	yaw = 0.0f; //Rotation
	roll = 0.0f;
}
