#pragma once
#include "Window/Window.h"
#include "Other/SoupTimer.h"

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
};

