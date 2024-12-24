#include "Scene.h"

#include "SceneContext.h"

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
	// TODO: Create a skybox

	// INFO: Reference the scene in the scene context
	SceneContext::SetScene(this);

	return true;
}

void Scene::Start()
{
	// INFO: Start all game objects
	for (auto& gameObject : gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update(float deltaTime)
{
	// INFO: Update all game objects
	for (auto& gameObject : gameObjects)
	{
		gameObject->Update(deltaTime);
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
