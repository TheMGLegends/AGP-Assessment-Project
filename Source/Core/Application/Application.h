#pragma once

#include <memory>

//#include "../../Scene/Scene.h"
//#include "../Renderer/Renderer.h"
#include "Window.h"

class Application
{
public:
	Application(HINSTANCE hInstance, int nCmdShow, const WindowInfo& windowInfo);
	~Application();

	void Run();

	void HandleInput();
	void Update(float deltaTime);
	void RenderFrame();

	void SetIsRunning(const bool _isRunning) { isRunning = _isRunning; }

private:
	Window window;
	//std::unique_ptr<Renderer> renderer;
	//std::unique_ptr<Scene> currentScene;

	bool isRunning;
};

