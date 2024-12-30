#pragma once

#include <memory>
#include <vector>

class Camera;
class GameObject;
class Skybox;
class UserInterfaceElement;

class Scene
{
	friend class Renderer;
	friend class SceneContext;

public:
	Scene();
	virtual ~Scene() = 0;

	virtual bool Initialise();

	void Start();
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void ProcessDestroyedGameObjects();

	Camera* GetCamera() const;

protected:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<UserInterfaceElement>> uiElements;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Skybox> skybox;
};

