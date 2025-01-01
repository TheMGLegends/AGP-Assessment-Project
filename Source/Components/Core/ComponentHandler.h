#pragma once

#include <DirectXCollision.h>
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
	/// @brief Handles the collision between two colliders where at least one of them is a trigger
	static void HandleTriggerCollision(std::shared_ptr<Collider> collider1, std::shared_ptr<Collider> collider2);


	// INFO: These functions handle the reverting of positions when two non-trigger colliders collide

	static void HandleBoxBox(DirectX::BoundingOrientedBox& box1, DirectX::BoundingOrientedBox box2);
	static void HandleSphereSphere(DirectX::BoundingSphere& sphere1, DirectX::BoundingSphere sphere2);
	static void HandleBoxSphere(DirectX::BoundingOrientedBox& box, DirectX::BoundingSphere& sphere);

private:
	static std::vector<std::weak_ptr<Collider>> colliders;
	static std::vector<std::weak_ptr<Emitter>> emitters;
	static std::vector<std::weak_ptr<Light>> lights;
	static std::vector<std::weak_ptr<Mesh>> meshes;
	static std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};

