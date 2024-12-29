#pragma once

#include <memory>
#include <vector>

class Collider;
class Emitter;
class Light;
class Mesh;
class Rigidbody;

class ComponentHandler
{
	friend class Renderer;

	friend class Collider;
	friend class Emitter;
	friend class Light;
	friend class Mesh;
	friend class Rigidbody;

public:
	ComponentHandler() = delete;
	~ComponentHandler() = delete;
	ComponentHandler(const ComponentHandler&) = delete;
	ComponentHandler& operator=(const ComponentHandler&) = delete;

	static void Update(float deltaTime);
	static void Draw();

	static void CheckCollisions();

	static void ClearExpired();

private:
	static std::vector<std::weak_ptr<Collider>> colliders;
	static std::vector<std::weak_ptr<Emitter>> emitters;
	static std::vector<std::weak_ptr<Light>> lights;
	static std::vector<std::weak_ptr<Mesh>> meshes;
	static std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};

