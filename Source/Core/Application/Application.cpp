#include "Application.h"

#include <iostream>

#include "../Time/Time.h"

Application::Application(HINSTANCE hInstance, int nCmdShow, const WindowInfo& windowInfo) : window(), isRunning(false)
{
	// INFO: Initialise the window
	if (FAILED(window.Initialise(hInstance, nCmdShow, windowInfo)))
	{
		std::cout << "Application::Application(): Failed to initialise the window!" << std::endl;
		return;
	}

	// INFO: Initialise the renderer
	//renderer = std::make_unique<Renderer>();
	//if (FAILED(renderer->Initialise(window.GetWindowHandle())))
	//{
	//	std::cout << "Application::Application(): Failed to initialise the renderer!" << std::endl;
	//	return;
	//}

	// INFO: Initialise the scene
	//currentScene = std::make_unique<Scene>();
	//if (!currentScene->Initialise())
	//{
	//	std::cout << "Application::Application(): Failed to initialise the scene!" << std::endl;
	//	return;
	//}

	// INFO: Set the window's quit callback
	window.SetOnQuit([this]() { isRunning = false; });

	// INFO: Set the application to running
	isRunning = true;
}

Application::~Application()
{
	// TODO: Cleanup the application
}

void Application::Run()
{
	// TODO: Handle start via scene

	// INFO: Run the application loop
	while (isRunning)
	{
		Time::Tick();

		// INFO: Terminate if quit message received
		if (Window::ProcessMessages() == -1)
			break;

		HandleInput();
		Update(Time::GetDeltaTime());

		// TODO: Handle collision detection via ComponentHandler (CollisionHandler)

		RenderFrame();
	}
}

void Application::HandleInput()
{
	// TODO: Handle input via InputHandler
}

void Application::Update(float deltaTime)
{
	// TODO: Handle updating via current scene
}

void Application::RenderFrame()
{
	// TODO: Handle rendering via renderer
}
