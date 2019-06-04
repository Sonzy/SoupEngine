#include "Application.h"
#include <sstream>
#include <iomanip>


Application::Application()
	:window(800, 600, "Soup Window")
{
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
	window.GetGraphics().ClearBuffer(0.0f, 1.0f, 0.0f);

	window.GetGraphics().DrawTestTriangle(timer.Peek());



	window.GetGraphics().EndFrame();

}
