#pragma once

#include <memory>
#include <vector>

class Collider;
class Emitter;
class Light;
class Rigidbody;

class ComponentHandler
{
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
	static std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};

