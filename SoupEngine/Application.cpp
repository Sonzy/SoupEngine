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
	const float time = timer.Peek();
	std::ostringstream oss;
	oss << "Time Elapsed: " << std::setprecision(1) << std::fixed << time << "s";
	window.SetWindowTitle(oss.str());
}
