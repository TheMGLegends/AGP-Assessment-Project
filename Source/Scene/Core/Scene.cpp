#include "Scene.h"

#include <DirectXMath.h>

#include "SceneContext.h"
#include "../../Game/Camera/Camera.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Core/Renderer/Skybox.h"
#include "../../UI/Core/UserInterfaceElement.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace DebugUtils;
using namespace DirectX;

Scene::Scene() : gameObjects(), uiElements(), camera(nullptr), skybox(nullptr), ambientLight(), directionalLight(), pointLights()
{
}

Scene::~Scene()
{
}

bool Scene::Initialise()
{
	camera = std::make_unique<Camera>();
	// TEST CODE
	camera->GetFreeCamInfo().isFreeCam = true;

    // TODO: Pick proper material and model
	skybox = std::make_unique<Skybox>("Cube", "GalaxySkyboxMaterial");

	// INFO: Default ambient light
	ambientLight.SetColour(XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f));

	// INFO: Default directional light
	directionalLight.SetColour(XMVectorSet(0.2788f, 0.7063f, 0.6506f, 1.0f));
	directionalLight.SetDirection(XMVectorSet(0.96f, 0.8f, 0.75f, 0.0f));

	// INFO: Reference the scene in the scene context
	SceneContext::SetScene(this);

	return true;
}

void Scene::Start()
{
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject)
		{
			LogWarning("Scene::Start(): GameObject is nullptr!");
			continue;
		}

		if (!gameObject->GetIsActive())
			continue;

		gameObject->Start();
	}
}

void Scene::Update(float deltaTime)
{
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject)
		{
			LogWarning("Scene::Update(): GameObject is nullptr!");
			continue;
		}

		if (!gameObject->GetIsActive())
			continue;

		gameObject->Update(deltaTime);
	}

	for (auto& uiElement : uiElements)
	{
		if (!uiElement)
		{
			LogWarning("Scene::Update(): UIElement is nullptr!");
			continue;
		}

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
		if (!gameObject)
		{
			LogWarning("Scene::LateUpdate(): GameObject is nullptr!");
			continue;
		}

		if (!gameObject->GetIsActive())
			continue;

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

const std::vector<std::unique_ptr<UserInterfaceElement>>& Scene::GetUserInterfaceElements() const
{
	return uiElements;
}

Camera* Scene::GetCamera() const
{
	return camera.get();
}

Skybox* Scene::GetSkybox() const
{
	return skybox.get();
}

void Scene::AddPointLight(const PointLight& pointLight)
{
	for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		// INFO: Overrides the first disabled point light
		if (!pointLights[i].GetIsEnabled())
		{
			pointLights[i] = pointLight;

			// INFO: Extra security to ensure the point light is enabled
			pointLights[i].SetIsEnabled(TRUE);
			return;
		}
	}

	// INFO: Given that no point light was disabled, the first point light is overridden
	pointLights[0] = pointLight;
	pointLights[0].SetIsEnabled(TRUE);

	LogWarning("Scene::AddPointLight(): No disabled point light found, overriding the first point light!");
}
