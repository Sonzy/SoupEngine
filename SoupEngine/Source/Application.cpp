#include "Application.h"
#include <sstream>
#include <iomanip>

#include <random>
#include "DirectXStuff/Drawable/SoupMath.h"

#include "DirectXStuff/Drawable/Melon.h"
#include "DirectXStuff/Drawable/Box.h"
#include "DirectXStuff/Drawable/Pyramid.h"
#include "DirectXStuff/Drawable/Sheet.h"
#include "DirectXStuff/Drawable/SkinnedBox.h"

#include "DirectXStuff/Textures/GDIPlusManager.h"
#include "DirectXStuff/Textures/Surface.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

GDIPlusManager gdipm;

Application::Application()
	:window(800, 600, "Soup Window")
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			: gfx(gfx)
		{}

		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(gfx, rng, adist, ddist, odist, rdist);
			case 1:
				return std::make_unique<SkinnedBox>(gfx, rng, adist, ddist,odist, rdist);
			case 2:
				return std::make_unique<Melon>(gfx, rng, adist, ddist,odist, rdist, longdist, latdist);
			case 3:
				return std::make_unique<Sheet>(gfx, rng, adist, ddist, odist, rdist);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}	

	private:
			Graphics & gfx;
			std::mt19937 rng{ std::random_device{}() };
			std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
			std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
			std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
			std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
			std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
			std::uniform_int_distribution<int> latdist{ 5,20 };
			std::uniform_int_distribution<int> longdist{ 10,40 };
			std::uniform_int_distribution<int> typedist{ 0,2 };
	};

	Factory f(window.GetGraphics());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

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

	window.GetGraphics().BeginFrame(0.0f, 0.0f, 1.0f, 1.0f);
	window.GetGraphics().SetCamera(cam.GetMatrix());

	for (auto& d : drawables)
	{
		d->Update(window.keyboard.IsKeyPressed(VK_SPACE) ? 0.0f : deltaTime);
		d->Draw(window.GetGraphics());
	}

	//Render simspeed window
	if (ImGui::Begin("Sim Speed"))
	{
		ImGui::SliderFloat("Speed", &simSpeed, 0.0f, 5.0f);
		ImGui::Text("App Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	cam.SpawnControlWindow();

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

