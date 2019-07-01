#include "Application.h"
#include <sstream>
#include <iomanip>

#include <random>
#include "DirectXStuff/Drawable/SoupMath.h"

#include "DirectXStuff/Drawable/Melon.h"
#include "DirectXStuff/Drawable/Box.h"
#include "DirectXStuff/Drawable/Pyramid.h"
#include "DirectXStuff/Drawable/Sheet.h"

//#include "DirectXStuff/Textures/GDIPlusManager.h"
//#include "DirectXStuff/Textures/Surface.h"


//GDIPlusManager gdipm;

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
				return std::make_unique<Box>(gfx, rng, adist, ddist,odist, rdist, bdist);
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

	//const auto s = Surface::FromFile("Images\\kappa50.png");

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
	auto deltaTime = timer.Mark();
	window.GetGraphics().ClearBuffer(0.0f, 0.0f, 1.0f);

	for (auto& d : drawables)
	{
		d->Update(window.keyboard.IsKeyPressed(VK_SPACE) ? 0.0f : deltaTime);
		d->Draw(window.GetGraphics());
	}

	window.GetGraphics().EndFrame();
}
