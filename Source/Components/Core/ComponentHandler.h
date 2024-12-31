#pragma once

#include <memory>
#include <vector>

#include "Component.h"

class Collider;
class Emitter;
class Light;
class Mesh;
class Rigidbody;

class ComponentHandler
{
public:
	ComponentHandler() = delete;
	~ComponentHandler() = delete;
	ComponentHandler(const ComponentHandler&) = delete;
	ComponentHandler& operator=(const ComponentHandler&) = delete;

	static void Update(float deltaTime);
	static void CheckCollisions();

	static void ClearExpired();

	static void RegisterComponent(const std::shared_ptr<Component>& component);

	static const std::vector<std::weak_ptr<Mesh>>& GetMeshes();

private:
	static std::vector<std::weak_ptr<Collider>> colliders;
	static std::vector<std::weak_ptr<Emitter>> emitters;
	static std::vector<std::weak_ptr<Light>> lights;
	static std::vector<std::weak_ptr<Mesh>> meshes;
	static std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};

