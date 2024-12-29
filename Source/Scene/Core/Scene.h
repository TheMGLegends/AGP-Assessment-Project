#pragma once

#include <memory>
#include <vector>

#include "../../Game/Camera/Camera.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Core/Renderer/Skybox.h"
#include "../../UI/Core/UserInterfaceElement.h"

class Scene
{
	friend class Renderer;
	friend class SceneContext;

public:
	Scene() = default;
	virtual ~Scene() = 0;

	virtual bool Initialise();

	void Start();
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void ProcessDestroyedGameObjects();

protected:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<UserInterfaceElement>> uiElements;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Skybox> skybox;
};

