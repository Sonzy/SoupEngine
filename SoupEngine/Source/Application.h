#pragma once
#include "Window/Window.h"
#include "Other/SoupTimer.h"
#include "DirectXStuff/Drawable/Box.h"

class Application
{
public:
	Application();

	int InitGameLoop();

private:
	// Runs a single frame
	void Tick();
	Window window;

	SoupTimer timer;

	std::vector<std::unique_ptr<Box>> boxes;
};

