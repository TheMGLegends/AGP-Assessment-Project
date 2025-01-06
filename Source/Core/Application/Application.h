#pragma once

#include <memory>

#include "Window.h"

class Renderer;
class Scene;

class Application
{
public:
	Application(HINSTANCE hInstance, int nCmdShow, const WindowInfo& windowInfo);
	~Application();

	void Run();

	void Update(float deltaTime);
	void RenderFrame();

	void SetIsRunning(bool _isRunning) { isRunning = _isRunning; }

private:
	void SwitchMouseMode();

	/// @brief Enables collider wireframes and changes skybox to debug mode
	void SwitchDebugMode();

	/// @brief Switch between freeCam and target
	void SwitchCameraMode();

private:
	Window window;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Scene> currentScene;

	bool isRunning;
	bool isFirstUpdate;
};

