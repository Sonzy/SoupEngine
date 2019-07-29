#pragma once
#include "Window/Window.h"
#include "Other/SoupTimer.h"
#include "DirectXStuff/Drawable/Drawable.h"
#include "ImGui/ImguiManager.h"
#include "Other/Camera.h"
#include "DirectXStuff/PointLight/PointLight.h"
#include <set>

class Box;

class Application
{
public:
	Application();

	int InitGameLoop();

private:
	// Runs a single frame
	void Tick();
	void RenderDemoWindow();
	void RenderBoxMaterialControlWindow();

	ImguiManager imGuiManager;
	SoupTimer timer;
	std::vector<std::unique_ptr<Drawable>> drawables;
	Window window;
	static constexpr size_t nDrawables = 180;
	Camera cam;
	PointLight light;
	float simSpeed = 1.0f;
	std::vector<Box*> boxes;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
};

