#include "ComponentHandler.h"

//#include "../../Components/Lighting/Light.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Emitter/Emitter.h"
//#include "../../Components/Physics/Collider.h"
//#include "../../Components/Physics/Rigidbody.h"

std::vector<std::weak_ptr<Collider>> ComponentHandler::colliders;
std::vector<std::weak_ptr<Emitter>> ComponentHandler::emitters;
std::vector<std::weak_ptr<Light>> ComponentHandler::lights;
std::vector<std::weak_ptr<Mesh>> ComponentHandler::meshes;
std::vector<std::weak_ptr<Rigidbody>> ComponentHandler::rigidbodies;

void ComponentHandler::Update(float deltaTime)
{
	// TODO: Order of component updates here

	// INFO: Update Emitters
	for (const auto& e : emitters)
	{
		if (auto emitter = e.lock())
			emitter->Update(deltaTime);
	}
}

void ComponentHandler::CheckCollisions()
{
	// TODO: Collision checking here
}

void ComponentHandler::ClearExpired()
{
	// INFO: Clear all expired colliders
	colliders.erase(
		std::remove_if(colliders.begin(), colliders.end(), 
		[](const std::weak_ptr<Collider>& collider) { return collider.expired(); }), 
	colliders.end());

	// INFO: Clear all expired emitters
	emitters.erase(
		std::remove_if(emitters.begin(), emitters.end(),
		[](const std::weak_ptr<Emitter>& emitter) { return emitter.expired(); }),
	emitters.end());

	// INFO: Clear all expired lights
	lights.erase(
		std::remove_if(lights.begin(), lights.end(),
		[](const std::weak_ptr<Light>& light) { return light.expired(); }),
	lights.end());

	// INFO: Clear all expired meshes
	meshes.erase(
		std::remove_if(meshes.begin(), meshes.end(),
		[](const std::weak_ptr<Mesh>& mesh) { return mesh.expired(); }),
	meshes.end());

	// INFO: Clear all expired rigidbodies
	rigidbodies.erase(
		std::remove_if(rigidbodies.begin(), rigidbodies.end(),
		[](const std::weak_ptr<Rigidbody>& rigidbody) { return rigidbody.expired(); }),
	rigidbodies.end());
}
