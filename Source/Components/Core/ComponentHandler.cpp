#include "ComponentHandler.h"

//#include "../../Components/Lighting/Light.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Emitter/Emitter.h"
#include "../../Components/Physics/BoxCollider.h"
#include "../../Components/Physics/SphereCollider.h"
#include "../../Components/Physics/Rigidbody.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Utilities/Debugging/DebugUtils.h"

std::vector<std::weak_ptr<Collider>> ComponentHandler::colliders;
std::vector<std::weak_ptr<Emitter>> ComponentHandler::emitters;
std::vector<std::weak_ptr<Light>> ComponentHandler::lights;
std::vector<std::weak_ptr<Mesh>> ComponentHandler::meshes;
std::vector<std::weak_ptr<Rigidbody>> ComponentHandler::rigidbodies;

using namespace DebugUtils;
using namespace DirectX;

void ComponentHandler::Update(float deltaTime)
{
	// TODO: Order of component updates here

	// INFO: Update Rigidbodies
	for (const auto& rb : rigidbodies)
	{
		if (auto rigidbody = rb.lock())
			rigidbody->Update(deltaTime);
	}

	// INFO: update Colliders
	for (const auto& c : colliders)
	{
		if (auto collider = c.lock())
			collider->Update(deltaTime);
	}

	// INFO: Update Emitters
	for (const auto& e : emitters)
	{
		if (auto emitter = e.lock())
			emitter->Update(deltaTime);
	}
}

void ComponentHandler::CheckCollisions()
{
	// INFO: Check for collisions between colliders
	for (size_t i = 0; i < colliders.size(); ++i)
	{
		auto collider1 = colliders[i].lock();

		if (!collider1 || !collider1->GetIsActive())
			continue;

		for (size_t j = i + 1; j < colliders.size(); ++j)
		{
			auto collider2 = colliders[j].lock();

			if (!collider2 || !collider2->GetIsActive())
				continue;

			// INFO: Check for collision between the two colliders
			if (collider1->GetColliderType() == Collider::Type::Box && collider2->GetColliderType() == Collider::Type::Box)
			{
				auto box1 = std::dynamic_pointer_cast<BoxCollider>(collider1);
				auto box2 = std::dynamic_pointer_cast<BoxCollider>(collider2);

				if (!box1 || !box2)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}

				// INFO: If a collision did occur
				if (box1->GetOrientedBox().Contains(box2->GetOrientedBox()) != DirectX::DISJOINT)
				{
					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
						HandleTriggerCollision(collider1, collider2);
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveBoxBox(box1, box2);
					}
				}
			}
			else if (collider1->GetColliderType() == Collider::Type::Sphere && collider2->GetColliderType() == Collider::Type::Sphere)
			{
				auto sphere1 = std::dynamic_pointer_cast<SphereCollider>(collider1);
				auto sphere2 = std::dynamic_pointer_cast<SphereCollider>(collider2);

				if (!sphere1 || !sphere2)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}

				// INFO: If a collision did occur
				if (sphere1->GetSphere().Contains(sphere2->GetSphere()) != DirectX::DISJOINT)
				{
					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
						HandleTriggerCollision(collider1, collider2);
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveSphereSphere(sphere1, sphere2);
					}
				}
			}
			else if (collider1->GetColliderType() == Collider::Type::Box && collider2->GetColliderType() == Collider::Type::Sphere)
			{
				auto box = std::dynamic_pointer_cast<BoxCollider>(collider1);
				auto sphere = std::dynamic_pointer_cast<SphereCollider>(collider2);

				if (!box || !sphere)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}
				
				// INFO: If a collision did occur
				if (box->GetOrientedBox().Contains(sphere->GetSphere()) != DirectX::DISJOINT)
				{
					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
						HandleTriggerCollision(collider1, collider2);
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveBoxSphere(box, sphere);
					}
				}
			}
			else if (collider1->GetColliderType() == Collider::Type::Sphere && collider2->GetColliderType() == Collider::Type::Box)
			{
				auto sphere = std::dynamic_pointer_cast<SphereCollider>(collider1);
				auto box = std::dynamic_pointer_cast<BoxCollider>(collider2);

				if (!sphere || !box)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}
				
				// INFO: If a collision did occur
				if (sphere->GetSphere().Contains(box->GetOrientedBox()) == DirectX::DISJOINT)
				{
					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
						HandleTriggerCollision(collider1, collider2);
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveBoxSphere(box, sphere);
					}
				}
			}
		}
	}
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

void ComponentHandler::RegisterComponent(const std::shared_ptr<Component>& component)
{
	// TODO: This stuff right here

	// INFO: Register the component
	if (std::shared_ptr<Collider> collider = std::dynamic_pointer_cast<Collider>(component))
		colliders.push_back(collider);
	else if (std::shared_ptr<Emitter> emitter = std::dynamic_pointer_cast<Emitter>(component))
		emitters.push_back(emitter);
	//else if (std::shared_ptr<Light> light = std::dynamic_pointer_cast<Light>(component))
	//	lights.push_back(light);
	else if (std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(component))
		meshes.push_back(mesh);
	else if (std::shared_ptr<Rigidbody> rigidbody = std::dynamic_pointer_cast<Rigidbody>(component))
		rigidbodies.push_back(rigidbody);
}

const std::vector<std::weak_ptr<Mesh>>& ComponentHandler::GetMeshes()
{
	return meshes;
}

void ComponentHandler::HandleTriggerCollision(std::shared_ptr<Collider> collider1, std::shared_ptr<Collider> collider2)
{
	if (collider1->GetIsTrigger() && collider2->GetIsTrigger())
	{
		collider1->ExecuteOnTrigger(collider2);
		collider2->ExecuteOnTrigger(collider1);
	}
	else if (collider1->GetIsTrigger())
	{
		collider1->ExecuteOnTrigger(collider2);
		collider2->ExecuteOnCollision(collider1);
	}
	else if (collider2->GetIsTrigger())
	{
		collider1->ExecuteOnCollision(collider2);
		collider2->ExecuteOnTrigger(collider1);
	}
}

void ComponentHandler::ResolveBoxBox(std::shared_ptr<BoxCollider>& box1, std::shared_ptr<BoxCollider>& box2)
{
	/*
	* 1. If both are static (No rigidbodies) then we just return
	* 2. If one is static and the other is dynamic then we:
	*		a). Figure out which previous axis are no longer colliding by individually checking each previous axis position
	*		b). Revert the dynamic object's position on those axis only
	* 3. If both are dynamic then we:
	* 		a). Figure out which previous axis are no longer colliding by individually checking each previous axis position
	* 		b). Revert both dynamic object's position on those axis only
	*/

	auto rb1 = box1->GetGameObject()->GetComponent<Rigidbody>().lock();
	auto rb2 = box2->GetGameObject()->GetComponent<Rigidbody>().lock();

	// INFO: Both objects are static
	if (!rb1 && !rb2)
		return;

	BoundingOrientedBox& orientedBox1 = box1->GetOrientedBox();
	BoundingOrientedBox& orientedBox2 = box2->GetOrientedBox();

	auto transform1 = box1->GetGameObject()->transform.lock();
	auto transform2 = box2->GetGameObject()->transform.lock();

	// INFO: One axis at a time checks

}

void ComponentHandler::ResolveSphereSphere(std::shared_ptr<SphereCollider>& sphere1, std::shared_ptr<SphereCollider>& sphere2)
{
	/*
	* 1. If both are static (No rigidbodies) then we just return
	* 2. If one is static and the other is dynamic then we:
	*		a). Figure out which previous axis are no longer colliding by individually checking each previous axis position
	*		b). Revert the dynamic object's position on those axis only
	* 3. If both are dynamic then we:
	* 		a). Figure out which previous axis are no longer colliding by individually checking each previous axis position
	* 		b). Revert both dynamic object's position on those axis only
	*/
}

void ComponentHandler::ResolveBoxSphere(std::shared_ptr<BoxCollider>& box, std::shared_ptr<SphereCollider>& sphere)
{
	/*
	* 1. If both are static (No rigidbodies) then we just return
	* 2. If one is static and the other is dynamic then we:
	*		a). Figure out which previous axis are no longer colliding by individually checking each previous axis position
	*		b). Revert the dynamic object's position on those axis only
	* 3. If both are dynamic then we:
	* 		a). Figure out which previous axis are no longer colliding by individually checking each previous axis position
	* 		b). Revert both dynamic object's position on those axis only
	*/
}
