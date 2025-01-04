#include "Application.h"

#include "../Input/InputHandler.h"
#include "../Renderer/Renderer.h"
#include "../Time/Time.h"
#include "../../Components/Core/ComponentHandler.h"
#include "../../Game/Camera/Camera.h"
#include "../../Scene/GameScene.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace DebugUtils;

Application::Application(HINSTANCE hInstance, int nCmdShow, const WindowInfo& windowInfo) : window(), isRunning(false)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	// INFO: Initialise the window
	if (FAILED(window.Initialise(hInstance, nCmdShow, windowInfo)))
	{
		LogError("Application::Application(): Failed to initialise the window!");
		return;
	}

	// INFO: Initialise the renderer
	renderer = std::make_unique<Renderer>();
	if (FAILED(renderer->Initialise(window.GetWindowHandle())))
	{
		LogError("Application::Application(): Failed to initialise the renderer!");
		return;
	}

	// INFO: Initialise the scene
	currentScene = std::make_unique<GameScene>();
	if (!currentScene->Initialise())
	{
		LogError("Application::Application(): Failed to initialise the scene!");
		return;
	}

	// INFO: Set the window's quit callback
	window.SetOnQuit([this]() { isRunning = false; });

	isRunning = true;
}

Application::~Application()
{
}

void Application::Run()
{
	currentScene->Start();

	// INFO: Run the application loop
	while (isRunning)
	{
		Time::Tick();

		// INFO: Terminate if quit message received
		if (Window::ProcessMessages() == -1)
			break;

		// INFO: Update camera aspect ratio to match window size
		currentScene->GetCamera()->GetProjectionInfo().aspectRatio = window.GetAspectRatio();

		InputHandler::HandleInput();
		Update(Time::GetDeltaTime());
		ComponentHandler::Update(Time::GetDeltaTime());
		ComponentHandler::CheckCollisions();
		RenderFrame();

		currentScene->ProcessDestroyedGameObjects();
		ComponentHandler::ClearExpired();
	}
}

void Application::Update(float deltaTime)
{
	currentScene->Update(deltaTime);
	currentScene->LateUpdate(deltaTime);
}

void Application::RenderFrame()
{
	renderer->RenderFrame(currentScene.get());
}
