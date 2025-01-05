#pragma once

#include <memory>
#include <vector>

#include "Component.h"

class BoxCollider;
class Collider;
class Emitter;
class Mesh;
class SphereCollider;
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
	static const std::vector<std::weak_ptr<Collider>>& GetColliders();

private:
	static void ResolveBoxBox(std::shared_ptr<BoxCollider>& box1, std::shared_ptr<BoxCollider>& box2);
	static void ResolveSphereSphere(std::shared_ptr<SphereCollider>& sphere1, std::shared_ptr<SphereCollider>& sphere2);
	static void ResolveBoxSphere(std::shared_ptr<BoxCollider>& box, std::shared_ptr<SphereCollider>& sphere);

private:
	static std::vector<std::weak_ptr<Collider>> colliders;
	static std::vector<std::weak_ptr<Emitter>> emitters;
	static std::vector<std::weak_ptr<Mesh>> meshes;
	static std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};

