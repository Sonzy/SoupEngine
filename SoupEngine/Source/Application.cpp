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
	:window(1600, 900, "Soup Window"), light(window.GetGraphics())
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

	nano.Draw(window.GetGraphics());
	light.Draw(window.GetGraphics());

	RawInputToggle();
	CameraMovement(deltaTime);

	//Render simspeed window
	if (ImGui::Begin("Sim Speed"))
	{
		ImGui::SliderFloat("Speed", &simSpeed, 0.0f, 5.0f);
		ImGui::Text("App Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	nano.ShowWindow();

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

void Application::RawInputToggle()
{
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

void Application::CameraMovement(float deltaTime)
{
	if (window.CursorEnabled())
		return;

	if (window.keyboard.IsKeyPressed('W'))
		cam.Translate( {0.0f, 0.0f, deltaTime } );
	if (window.keyboard.IsKeyPressed('A'))
		cam.Translate({ -deltaTime, 0.0f, 0.0f });
	if (window.keyboard.IsKeyPressed('S'))
		cam.Translate({ 0.0f, 0.0f, -deltaTime });
	if (window.keyboard.IsKeyPressed('D'))
		cam.Translate({ deltaTime, 0.0f, 0.0f });

	if (window.keyboard.IsKeyPressed('R'))
		cam.Translate({ 0.0f, deltaTime, 0.0f });
	if (window.keyboard.IsKeyPressed('F'))
		cam.Translate({ 0.0f, -deltaTime, 0.0f });

	while (const auto delta = window.mouse.ReadRawDelta())
	{
		if (!window.CursorEnabled())
			cam.Rotate(delta->x, delta->y);
	}
}

