#pragma once

#include <SimpleMath.h>

#include "Scene.h"
#include "../../Components/Transform/Transform.h"
#include "../../Game/GameObjects/Core/GameObject.h"

class SceneContext
{
public:
	SceneContext() = delete;
	~SceneContext() = delete;
	SceneContext(const SceneContext&) = delete;
	SceneContext& operator=(const SceneContext&) = delete;

	static inline void SetScene(Scene* scene) { sceneRef = scene; }
	static inline Scene* GetScene() { return sceneRef; }

	template<class T, typename... Args>
	static T* SpawnGameObject(const DirectX::SimpleMath::Vector3& spawnLocation, const DirectX::SimpleMath::Quaternion& spawnRotation, Args&&... args);

private:
	static Scene* sceneRef;
};

template<class T, typename ...Args>
inline T* SceneContext::SpawnGameObject(const DirectX::SimpleMath::Vector3& spawnLocation, const DirectX::SimpleMath::Quaternion& spawnRotation, Args && ...args)
{
	// INFO: Ensure T is a derived class of GameObject
	static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject");

	// INFO: Add GameObject to Scene
	sceneRef->gameObjects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));

	// INFO: Get a reference to the newly created GameObject
	T* newGameObject = dynamic_cast<T*>(sceneRef->gameObjects.back().get());

	// INFO: Set the GameObject's transform properties
	if (std::shared_ptr<Transform> newGameObjectTransform = newGameObject->transform.lock())
	{
		newGameObjectTransform->SetPosition(spawnLocation);
		newGameObjectTransform->SetRotation(spawnRotation);
	}

	return newGameObject;
}
