#include "Application.h"
#include <sstream>
#include <iomanip>

#include <random>
#include "DirectXStuff/Drawable/SoupMath.h"

#include "DirectXStuff/Textures/GDIPlusManager.h"
#include "DirectXStuff/Textures/Surface.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"


GDIPlusManager gdipm;


Application::Application()
	:window(800, 600, "Soup Window"), light(window.GetGraphics())
{
	window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int Application::InitGameLoop()
{
	// Window loop - Check windows messages
	while (true)
	{
		if (const auto exitCode = Window::ProcessMessages())
			return *exitCode;

		Tick();
	}
}

void Application::Tick()
{	
	auto deltaTime = timer.Mark() * simSpeed;

	window.GetGraphics().BeginFrame(0.0f, 0.0f, 0.20f, 1.0f);
	window.GetGraphics().SetCamera(cam.GetMatrix());
	light.Bind(window.GetGraphics(), cam.GetMatrix());

	const auto modelTrans = DirectX::XMMatrixRotationRollPitchYaw(transform.rot.roll, transform.rot.pitch, transform.rot.yaw)
		* DirectX::XMMatrixTranslation(transform.pos.x, transform.pos.y, transform.pos.z);

	nano.Draw(window.GetGraphics(), modelTrans);
	light.Draw(window.GetGraphics());

	//Toggling cursor
	while (const auto e = window.keyboard.ReadKey())
	{
		if (e->IsPressEvent() && e->GetKeyCode() == VK_INSERT)
		{
			if (window.CursorEnabled())
			{
				window.DisableCursor();
				window.mouse.EnableRawInput(true);
			}
			else
			{
				window.EnableCursor();
				window.mouse.EnableRawInput(false);
			}
		}
	}

	//Render simspeed window
	if (ImGui::Begin("Sim Speed"))
	{
		ImGui::SliderFloat("Speed", &simSpeed, 0.0f, 5.0f);
		ImGui::Text("App Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowModelWindow();
	ShowRawInputWindow();

	window.GetGraphics().EndFrame();
}

void Application::RenderDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window && window.GetGraphics().IsGUIEnabled())
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}
}

void Application::ShowModelWindow()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &transform.rot.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &transform.rot.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &transform.rot.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &transform.pos.x, -60.0f, 60.0f);
		ImGui::SliderFloat("X", &transform.pos.y, -60.0f, 60.0f);
		ImGui::SliderFloat("Z", &transform.pos.z, -60.0f, 60.0f);
		if (ImGui::Button("Reset"))
		{
			transform.pos.x = 0.0f;
			transform.pos.y = 0.0f;
			transform.pos.z = 0.0f;
			transform.rot.roll = 0.0f;
			transform.rot.pitch = 0.0f;
			transform.rot.yaw = 0.0f;
		}
	}
	ImGui::End();
}

void Application::ShowRawInputWindow()
{
	while (const auto d = window.mouse.ReadRawDelta())
	{
		x += d->x;
		y += d->y;
	}

	if (ImGui::Begin("Raw Input"))
	{
		ImGui::Text("Cursor: %s", window.CursorEnabled() ? "Enabled" : "Disabled");
		ImGui::Text("Tally: (%d, %d)", x, y);
	}
	ImGui::End();
}

