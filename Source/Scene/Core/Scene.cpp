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
	
	// TODO: Create a directional light game object
	camera = std::make_unique<Camera>();
    // TODO: Pick proper material and model
	skybox = std::make_unique<Skybox>("Cube", "TestMaterial");

	// INFO: Reference the scene in the scene context
	SceneContext::SetScene(this);

	return true;
}

void Scene::Start()
{
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->GetIsActive())
			continue;

		gameObject->Start();
	}
}

void Scene::Update(float deltaTime)
{
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->GetIsActive())
			continue;

		gameObject->Update(deltaTime);
	}

	for (auto& uiElement : uiElements)
	{
		if (!uiElement->GetIsActive())
			continue;

		uiElement->Update(deltaTime);
	}

	camera->Update(deltaTime);
}

void Scene::LateUpdate(float deltaTime)
{
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
