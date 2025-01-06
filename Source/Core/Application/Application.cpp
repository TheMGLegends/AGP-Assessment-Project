#include "Application.h"

#include "../Input/InputHandler.h"
#include "../Renderer/Renderer.h"
#include "../Time/Time.h"
#include "../../Components/Core/ComponentHandler.h"
#include "../../Components/Physics/Collider.h"
#include "../../Game/Camera/Camera.h"
#include "../../Scene/GameScene.h"
#include "../../Utilities/Debugging/DebugUtils.h"
#include "../../Utilities/Globals/Globals.h"

using namespace DebugUtils;
using namespace DirectX;

Application::Application(HINSTANCE hInstance, int nCmdShow, const WindowInfo& windowInfo) : window(), isRunning(false), isFirstLoop(true)
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

	// INFO: Initialise the collider wireframes for debug rendering
	if (FAILED(Collider::InitialiseWireframes(renderer->GetDevice(), renderer->GetDeviceContext())))
	{
		LogError("Application::Application(): Failed to initialise the collider wireframes!");
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

	// INFO: Setup input handling application specific bindings
	InputHandler::BindKeyToAction(Keyboard::Keys::M, BindData(std::bind(&Application::SwitchMouseMode, this), ButtonState::Pressed));
	InputHandler::BindKeyToAction(Keyboard::Keys::F1, BindData(std::bind(&Application::SwitchDebugMode, this), ButtonState::Pressed));
	InputHandler::BindKeyToAction(Keyboard::Keys::F2, BindData(std::bind(&Application::SwitchCameraMode, this), ButtonState::Pressed));

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

		if (isFirstLoop)
		{
			isFirstLoop = false;
			continue;
		}

		// INFO: Terminate if quit message received
		if (Window::ProcessMessages() == -1)
			break;

		// INFO: Update camera aspect ratio to match window size
		currentScene->GetCamera()->GetProjectionInfo().aspectRatio = window.GetAspectRatio();

		InputHandler::HandleInput();
		Update(Time::GetDeltaTime());
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

	ComponentHandler::Update(Time::GetDeltaTime());
}

void Application::RenderFrame()
{
	renderer->RenderFrame(currentScene.get());
}

void Application::SwitchMouseMode()
{
	if (!InputHandler::IsMouseInsideWindow(window.GetWindowHandle()))
		return;

	if (InputHandler::GetMouseMode() == Mouse::Mode::MODE_ABSOLUTE)
		InputHandler::SetMouseMode(Mouse::Mode::MODE_RELATIVE);
	else
		InputHandler::SetMouseMode(Mouse::Mode::MODE_ABSOLUTE);
}

void Application::SwitchDebugMode()
{
	Globals::gIsInDebugMode = !Globals::gIsInDebugMode;

	currentScene->SwitchDebugMode();
}

void Application::SwitchCameraMode()
{
	Camera* camera = currentScene->GetCamera();
	camera->SetIsFreeCam(!camera->GetIsFreeCam());
}
