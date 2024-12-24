#include "ComponentHandler.h"

//#include "../../Components/Lighting/Light.h"
//#include "../../Components/Particles/Emitter.h"
//#include "../../Components/Physics/Collider.h"
//#include "../../Components/Physics/Rigidbody.h"

std::vector<std::weak_ptr<Collider>> ComponentHandler::colliders;
std::vector<std::weak_ptr<Emitter>> ComponentHandler::emitters;
std::vector<std::weak_ptr<Light>> ComponentHandler::lights;
std::vector<std::weak_ptr<Rigidbody>> ComponentHandler::rigidbodies;

void ComponentHandler::Update(float deltaTime)
{
	// TODO: Order of component updates here
}

void ComponentHandler::Draw()
{
	// TODO: Drawing of specific components here
}

void ComponentHandler::CheckCollisions()
{
	// TODO: Collision checking here
}

void ComponentHandler::ClearExpired()
{
	// TODO: Clear all expired components here
}
