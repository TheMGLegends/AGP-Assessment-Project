#include "Scene.h"

#include "SceneContext.h"
#include "../../Game/Camera/Camera.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Core/Renderer/Skybox.h"
#include "../../UI/Core/UserInterfaceElement.h"

Scene::Scene() : gameObjects(), uiElements(), camera(nullptr), skybox(nullptr)
{
}

Scene::~Scene()
{
}

bool Scene::Initialise()
{
	// INFO: Default implementation creates a directional light game object
	//       a camera and a skybox
	
	// INFO: Create a directional light game object
	// TODO: Create a directional light game object

	// INFO: Create a camera
	camera = std::make_unique<Camera>();

	if (!camera)
		return false;

	// INFO: Create a skybox
    // TODO: Pick proper material and model
	skybox = std::make_unique<Skybox>("Cube", "TestMaterial");

	// INFO: Reference the scene in the scene context
	SceneContext::SetScene(this);

	return true;
}

void Scene::Start()
{
	// INFO: Start all game objects
	for (auto& gameObject : gameObjects)
	{
		// INFO: Continue if the game object is inactive
		if (!gameObject->GetIsActive())
			continue;

		gameObject->Start();
	}
}

void Scene::Update(float deltaTime)
{
	// INFO: Update all game objects
	for (auto& gameObject : gameObjects)
	{
		// INFO: Continue if the game object is inactive
		if (!gameObject->GetIsActive())
			continue;

		gameObject->Update(deltaTime);
	}

	// INFO: Update all UI elements
	for (auto& uiElement : uiElements)
	{
		// INFO: Continue if the UI element is inactive
		if (!uiElement->GetIsActive())
			continue;

		uiElement->Update(deltaTime);
	}

	// INFO: Update the camera
	camera->Update(deltaTime);
}

void Scene::LateUpdate(float deltaTime)
{
	// INFO: Late update all game objects
	for (auto& gameObject : gameObjects)
	{
		gameObject->LateUpdate(deltaTime);
	}
}

void Scene::ProcessDestroyedGameObjects()
{
	// INFO: Go through all GOs and remove the ones that are marked for deletion
	gameObjects.erase(
		std::remove_if(gameObjects.begin(), gameObjects.end(),
		[](const std::unique_ptr<GameObject>& gameObject) { return gameObject->GetIsMarkedForDeletion(); }),
	gameObjects.end());
}

Camera* Scene::GetCamera() const
{
	return camera.get();
}
