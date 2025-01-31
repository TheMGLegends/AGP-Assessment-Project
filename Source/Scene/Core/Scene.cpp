#include "Scene.h"

#include <DirectXMath.h>

#include "SceneContext.h"
#include "../../Assets/Config/DirectXConfig.h"
#include "../../Assets/Material/Material.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Game/Camera/Camera.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Core/Renderer/Skybox.h"
#include "../../UI/Core/UserInterfaceElement.h"
#include "../../Utilities/Debugging/DebugUtils.h"
#include "../../Utilities/Globals/Globals.h"


using namespace DebugUtils;
using namespace DirectXConfig;
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
	skybox = std::make_unique<Skybox>("Cube", "GalaxySkyboxMaterial");

	// INFO: Default ambient light
	ambientLight.SetColour(XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f));

	// INFO: Default directional light
	directionalLight.SetColour(XMVectorSet(0.288f, 0.7063f, 0.6506f, 1.0f));
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
}

void Scene::LateUpdate(float deltaTime)
{
	camera->LateUpdate(deltaTime);

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

void Scene::SwitchDebugMode()
{
	if (Globals::gIsInDebugMode)
	{
		skybox->GetMesh()->GetMaterial()->SetTexture("DebugSkybox");

		// INFO: Go through all game objects and change reflected texture for all materials that are reflective
		for (auto& gameObject : gameObjects)
		{
			if (std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Mesh>().lock())
			{
				if (Material* material = mesh->GetMaterial())
				{
					if (material->HasConstantBuffer(ConstantBufferType::ReflectiveVS))
						material->SetReflectedTexture("DebugSkybox");
				}
			}
		}
	}
	else
	{
		skybox->GetMesh()->GetMaterial()->SetTexture("GalaxySkybox");

		// INFO: Go through all game objects and change reflected texture for all materials that are reflective
		for (auto& gameObject : gameObjects)
		{
			if (std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Mesh>().lock())
			{
				if (Material* material = mesh->GetMaterial())
				{
					if (material->HasConstantBuffer(ConstantBufferType::ReflectiveVS))
						material->SetReflectedTexture("GalaxySkybox");
				}
			}
		}
	}
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
	for (size_t i = 0; i < Globals::MAX_POINT_LIGHTS; ++i)
	{
		// INFO: Overrides the first disabled point light
		if (!pointLights[i].GetIsEnabled())
		{
			pointLights[i] = pointLight;

			// INFO: Extra security to ensure the point light is enabled
			pointLights[i].SetIsEnabled(true);
			return;
		}
	}

	// INFO: Given that no point light was disabled, the first point light is overridden
	pointLights[0] = pointLight;
	pointLights[0].SetIsEnabled(true);

	LogWarning("Scene::AddPointLight(): No disabled point light found, overriding the first point light!");
}
