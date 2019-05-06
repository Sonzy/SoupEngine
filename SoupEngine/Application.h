#pragma once
#include "Window.h"
#include "SoupTimer.h"

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

