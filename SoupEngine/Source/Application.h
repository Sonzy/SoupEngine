#pragma once
#include "Window/Window.h"
#include "Other/SoupTimer.h"
#include "DirectXStuff/Drawable/Drawable.h"
#include "ImGui/ImguiManager.h"
#include "Other/Camera.h"
#include "DirectXStuff/PointLight/PointLight.h"
#include <set>
#include "DirectXStuff/Drawable/Model.h"

class Application
{
public:
	Application();

	int InitGameLoop();

private:
	// Runs a single frame
	void Tick();
	void RenderDemoWindow();
	void ShowModelWindow();

	ImguiManager imGuiManager;
	SoupTimer timer;
	Window window;
	Camera cam;
	PointLight light;
	Model nano{window.GetGraphics(), "Source\\Models\\nanosuit.obj"};
	float simSpeed = 1.0f;


	struct
	{
		struct
		{
			float roll = 0.0f;
			float pitch = 0.0f;
			float yaw = 0.0f;
		} rot;

		struct 
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		} pos;
	} transform;
};