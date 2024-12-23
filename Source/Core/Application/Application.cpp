#include "Application.h"

#include <iostream>

#include "../../Scene/GameScene.h"
#include "../Time/Time.h"

Application::Application(HINSTANCE hInstance, int nCmdShow, const WindowInfo& windowInfo) : window(), isRunning(false)
{
	// INFO: Seed the random number generator
	srand(static_cast<unsigned int>(time(nullptr)));

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
	currentScene = std::make_unique<GameScene>();
	if (!currentScene->Initialise())
	{
		std::cout << "Application::Application(): Failed to initialise the scene!" << std::endl;
		return;
	}

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
	// INFO: Start the current scene
	currentScene->Start();

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

		// INFO: Process Destruction of Game Objects
		currentScene->ProcessDestroyedGameObjects();
	}
}

void Application::HandleInput()
{
	// TODO: Handle input via InputHandler
}

void Application::Update(float deltaTime)
{
	// INFO: Update the current scene
	currentScene->Update(deltaTime);

	// INFO: Late update the current scene
	currentScene->LateUpdate(deltaTime);
}

void Application::RenderFrame()
{
	// TODO: Handle rendering via renderer
}
