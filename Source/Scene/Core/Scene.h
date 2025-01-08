#pragma once

#include <array>
#include <memory>
#include <vector>

#include "../../Lighting/AmbientLight.h"
#include "../../Lighting/DirectionalLight.h"
#include "../../Lighting/PointLight.h"
#include "../../Utilities/Globals/Globals.h"

class Camera;
class GameObject;
class Skybox;
class UserInterfaceElement;

class Scene
{
	friend class SceneContext;

public:
	Scene();
	virtual ~Scene() = 0;

	virtual bool Initialise();

	void Start();
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void ProcessDestroyedGameObjects();
	void SwitchDebugMode();

	const std::vector<std::unique_ptr<UserInterfaceElement>>& GetUserInterfaceElements() const;

	Camera* GetCamera() const;
	Skybox* GetSkybox() const;

	inline const AmbientLight& GetAmbientLight() const { return ambientLight; }
	inline const DirectionalLight& GetDirectionalLight() const { return directionalLight; }

	void AddPointLight(const PointLight& pointLight);
	inline const std::array<PointLight, Globals::MAX_POINT_LIGHTS>& GetPointLights() const { return pointLights; }

protected:
	std::vector<std::unique_ptr<GameObject>> gameObjects;
	std::vector<std::unique_ptr<UserInterfaceElement>> uiElements;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Skybox> skybox;

	AmbientLight ambientLight;
	DirectionalLight directionalLight;

private:
	std::array<PointLight, Globals::MAX_POINT_LIGHTS> pointLights;
};

