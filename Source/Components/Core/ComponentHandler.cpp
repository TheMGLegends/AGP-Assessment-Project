#include "ComponentHandler.h"

#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Emitter/Emitter.h"
#include "../../Components/Physics/BoxCollider.h"
#include "../../Components/Physics/SphereCollider.h"
#include "../../Components/Physics/Rigidbody.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Utilities/Debugging/DebugUtils.h"

std::vector<std::weak_ptr<Collider>> ComponentHandler::colliders;
std::vector<std::weak_ptr<Emitter>> ComponentHandler::emitters;
std::vector<std::weak_ptr<Mesh>> ComponentHandler::meshes;
std::vector<std::weak_ptr<Rigidbody>> ComponentHandler::rigidbodies;

using namespace DebugUtils;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void ComponentHandler::Update(float deltaTime)
{
	// INFO: Update Rigidbodies
	for (const auto& rb : rigidbodies)
	{
		if (std::shared_ptr<Rigidbody> rigidbody = rb.lock())
			rigidbody->Update(deltaTime);
	}

	// INFO: update Colliders
	for (const auto& c : colliders)
	{
		if (std::shared_ptr<Collider> collider = c.lock())
			collider->Update(deltaTime);
	}

	// INFO: Update Emitters
	for (const auto& e : emitters)
	{
		if (std::shared_ptr<Emitter> emitter = e.lock())
			emitter->Update(deltaTime);
	}
}

void ComponentHandler::CheckCollisions()
{
	// INFO: Check for collisions between colliders
	for (size_t i = 0; i < colliders.size(); ++i)
	{
		std::shared_ptr<Collider> collider1 = colliders[i].lock();

		if (!collider1 || !collider1->GetIsActive())
			continue;

		for (size_t j = i + 1; j < colliders.size(); ++j)
		{
			std::shared_ptr<Collider> collider2 = colliders[j].lock();

			if (!collider2 || !collider2->GetIsActive())
				continue;

			// INFO: Check for collision between the two colliders
			if (collider1->GetColliderType() == Collider::Type::Box && collider2->GetColliderType() == Collider::Type::Box)
			{
				std::shared_ptr<BoxCollider> box1 = std::dynamic_pointer_cast<BoxCollider>(collider1);
				std::shared_ptr<BoxCollider> box2 = std::dynamic_pointer_cast<BoxCollider>(collider2);

				if (!box1 || !box2)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}

				// INFO: If a collision did occur
				if (box1->GetOrientedBox().Contains(box2->GetOrientedBox()) != DirectX::DISJOINT)
				{
					box1->SetWireframeColour(Colors::Red);
					box2->SetWireframeColour(Colors::Red);

					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
					{
						collider1->ExecuteOnTrigger(collider2);
						collider2->ExecuteOnTrigger(collider1);
					}
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveBoxBox(box1, box2);
					}
				}
				else
				{
					box1->SetWireframeColour(Colors::LimeGreen);
					box2->SetWireframeColour(Colors::LimeGreen);
				}
			}
			else if (collider1->GetColliderType() == Collider::Type::Sphere && collider2->GetColliderType() == Collider::Type::Sphere)
			{
				std::shared_ptr<SphereCollider> sphere1 = std::dynamic_pointer_cast<SphereCollider>(collider1);
				std::shared_ptr<SphereCollider> sphere2 = std::dynamic_pointer_cast<SphereCollider>(collider2);

				if (!sphere1 || !sphere2)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}

				// INFO: If a collision did occur
				if (sphere1->GetSphere().Contains(sphere2->GetSphere()) != DirectX::DISJOINT)
				{
					sphere1->SetWireframeColour(Colors::Red);
					sphere2->SetWireframeColour(Colors::Red);

					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
					{
						collider1->ExecuteOnTrigger(collider2);
						collider2->ExecuteOnTrigger(collider1);
					}
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveSphereSphere(sphere1, sphere2);
					}
				}
				else
				{
					sphere1->SetWireframeColour(Colors::LimeGreen);
					sphere2->SetWireframeColour(Colors::LimeGreen);
				}
			}
			else if (collider1->GetColliderType() == Collider::Type::Box && collider2->GetColliderType() == Collider::Type::Sphere)
			{
				std::shared_ptr<BoxCollider> box = std::dynamic_pointer_cast<BoxCollider>(collider1);
				std::shared_ptr<SphereCollider> sphere = std::dynamic_pointer_cast<SphereCollider>(collider2);

				if (!box || !sphere)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}
				
				// INFO: If a collision did occur
				if (box->GetOrientedBox().Contains(sphere->GetSphere()) != DirectX::DISJOINT)
				{
					box->SetWireframeColour(Colors::Red);
					sphere->SetWireframeColour(Colors::Red);

					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
					{
						collider1->ExecuteOnTrigger(collider2);
						collider2->ExecuteOnTrigger(collider1);
					}
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveBoxSphere(box, sphere);
					}
				}
				else
				{
					box->SetWireframeColour(Colors::LimeGreen);
					sphere->SetWireframeColour(Colors::LimeGreen);
				}
			}
			else if (collider1->GetColliderType() == Collider::Type::Sphere && collider2->GetColliderType() == Collider::Type::Box)
			{
				std::shared_ptr<SphereCollider> sphere = std::dynamic_pointer_cast<SphereCollider>(collider1);
				std::shared_ptr<BoxCollider> box = std::dynamic_pointer_cast<BoxCollider>(collider2);

				if (!sphere || !box)
				{
					LogWarning("ComponentHandler::CheckCollisions(): A collider cast failed!");
					continue;
				}
				
				// INFO: If a collision did occur
				if (sphere->GetSphere().Contains(box->GetOrientedBox()) != DirectX::DISJOINT)
				{
					sphere->SetWireframeColour(Colors::Red);
					box->SetWireframeColour(Colors::Red);

					// INFO: Handle via Trigger or Collision functions
					if (collider1->GetIsTrigger() || collider2->GetIsTrigger())
					{
						collider1->ExecuteOnTrigger(collider2);
						collider2->ExecuteOnTrigger(collider1);
					}
					else
					{
						collider1->ExecuteOnCollision(collider2);
						collider2->ExecuteOnCollision(collider1);

						ResolveBoxSphere(box, sphere);
					}
				}
				else
				{
					sphere->SetWireframeColour(Colors::LimeGreen);
					box->SetWireframeColour(Colors::LimeGreen);
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
	// INFO: Register the component
	if (std::shared_ptr<Collider> collider = std::dynamic_pointer_cast<Collider>(component))
		colliders.push_back(collider);
	else if (std::shared_ptr<Emitter> emitter = std::dynamic_pointer_cast<Emitter>(component))
		emitters.push_back(emitter);
	else if (std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(component))
		meshes.push_back(mesh);
	else if (std::shared_ptr<Rigidbody> rigidbody = std::dynamic_pointer_cast<Rigidbody>(component))
		rigidbodies.push_back(rigidbody);
}

const std::vector<std::weak_ptr<Mesh>>& ComponentHandler::GetMeshes()
{
	return meshes;
}

const std::vector<std::weak_ptr<Collider>>& ComponentHandler::GetColliders()
{
	return colliders;
}

void ComponentHandler::ResolveBoxBox(std::shared_ptr<BoxCollider>& box1, std::shared_ptr<BoxCollider>& box2)
{
	std::shared_ptr<Rigidbody> rb1 = box1->GetGameObject()->GetComponent<Rigidbody>().lock();
	std::shared_ptr<Rigidbody> rb2 = box2->GetGameObject()->GetComponent<Rigidbody>().lock();

	// INFO: Both objects are static
	if (!rb1 && !rb2)
		return;

	// INFO: One object is static
	if (!rb1 || !rb2)
	{
		std::shared_ptr<BoxCollider>& staticCollider = rb1 ? box2 : box1;
		std::shared_ptr<BoxCollider>& dynamicCollider = rb1 ? box1 : box2;

		std::shared_ptr<Transform> dynamicTransform = dynamicCollider->GetGameObject()->transform.lock();
		Vector3 dynamicPreviousPosition = dynamicCollider->GetPreviousPosition();
		Vector3 dynamicPosition = dynamicTransform->GetPosition();

		BoundingOrientedBox& staticOrientedBox = staticCollider->GetOrientedBox();
		BoundingOrientedBox& dynamicOrientedBox = dynamicCollider->GetOrientedBox();

		// INFO: Check for collision on each axis individually
		
		// INFO: X-axis
		dynamicOrientedBox.Center.x = dynamicPreviousPosition.x; // NOTE: Doesn't require changing back because it'll get updated in the next frame

		// INFO: By reverting a singular axis, if they are no longer colliding we revert the dynamic object's position on that axis only
		if (staticOrientedBox.Contains(dynamicOrientedBox) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPreviousPosition.x, dynamicPosition.y, dynamicPosition.z), false);

		// INFO: Y-axis
		dynamicOrientedBox.Center.y = dynamicPreviousPosition.y; 

		if (staticOrientedBox.Contains(dynamicOrientedBox) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPosition.x, dynamicPreviousPosition.y, dynamicPosition.z), false);

		// INFO: Z-axis
		dynamicOrientedBox.Center.z = dynamicPreviousPosition.z;

		if (staticOrientedBox.Contains(dynamicOrientedBox) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPosition.x, dynamicPosition.y, dynamicPreviousPosition.z), false);
	}
}

void ComponentHandler::ResolveSphereSphere(std::shared_ptr<SphereCollider>& sphere1, std::shared_ptr<SphereCollider>& sphere2)
{
	std::shared_ptr<Rigidbody> rb1 = sphere1->GetGameObject()->GetComponent<Rigidbody>().lock();
	std::shared_ptr<Rigidbody> rb2 = sphere2->GetGameObject()->GetComponent<Rigidbody>().lock();

	// INFO: Both objects are static
	if (!rb1 && !rb2)
		return;

	// INFO: One object is static
	if (!rb1 || !rb2)
	{
		std::shared_ptr<SphereCollider>& staticCollider = rb1 ? sphere2 : sphere1;
		std::shared_ptr<SphereCollider>& dynamicCollider = rb1 ? sphere1 : sphere2;

		std::shared_ptr<Transform> dynamicTransform = dynamicCollider->GetGameObject()->transform.lock();
		Vector3 dynamicPreviousPosition = dynamicCollider->GetPreviousPosition();
		Vector3 dynamicPosition = dynamicTransform->GetPosition();

		BoundingSphere& staticSphere = staticCollider->GetSphere();
		BoundingSphere& dynamicSphere = dynamicCollider->GetSphere();

		// INFO: Check for collision on each axis individually

		// INFO: X-axis
		dynamicSphere.Center.x = dynamicPreviousPosition.x; // NOTE: Doesn't require changing back because it'll get updated in the next frame

		// INFO: By reverting a singular axis, if they are no longer colliding we revert the dynamic object's position on that axis only
		if (staticSphere.Contains(dynamicSphere) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPreviousPosition.x, dynamicPosition.y, dynamicPosition.z), false);

		// INFO: Y-axis
		dynamicSphere.Center.y = dynamicPreviousPosition.y;

		if (staticSphere.Contains(dynamicSphere) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPosition.x, dynamicPreviousPosition.y, dynamicPosition.z), false);

		// INFO: Z-axis
		dynamicSphere.Center.z = dynamicPreviousPosition.z;

		if (staticSphere.Contains(dynamicSphere) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPosition.x, dynamicPosition.y, dynamicPreviousPosition.z), false);
	}
}

void ComponentHandler::ResolveBoxSphere(std::shared_ptr<BoxCollider>& box, std::shared_ptr<SphereCollider>& sphere)
{
	std::shared_ptr<Rigidbody> rbBox = box->GetGameObject()->GetComponent<Rigidbody>().lock();
	std::shared_ptr<Rigidbody> rbSphere = sphere->GetGameObject()->GetComponent<Rigidbody>().lock();

	// INFO: Both objects are static
	if (!rbBox && !rbSphere)
		return;

	// INFO: One object is static
	if (!rbBox || !rbSphere)
	{
		BoundingOrientedBox& orientedBox = box->GetOrientedBox();
		BoundingSphere& boundingSphere = sphere->GetSphere();

		std::shared_ptr<Transform> dynamicTransform = rbBox ? box->GetGameObject()->transform.lock() : sphere->GetGameObject()->transform.lock();
		Vector3 dynamicPreviousPosition = rbBox ? box->GetPreviousPosition() : sphere->GetPreviousPosition();
		Vector3 dynamicPosition = dynamicTransform->GetPosition();

		// INFO: Check for collision on each axis individually

		// INFO: X-axis
		if (rbBox)
			orientedBox.Center.x = dynamicPreviousPosition.x; // NOTE: Doesn't require changing back because it'll get updated in the next frame
		else
			boundingSphere.Center.x = dynamicPreviousPosition.x; // NOTE: Doesn't require changing back because it'll get updated in the next frame

		// INFO: By reverting a singular axis, if they are no longer colliding we revert the dynamic object's position on that axis only
		if (orientedBox.Contains(boundingSphere) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPreviousPosition.x, dynamicPosition.y, dynamicPosition.z), false);

		// INFO: Y-axis
		if (rbBox)
			orientedBox.Center.y = dynamicPreviousPosition.y;
		else
			boundingSphere.Center.y = dynamicPreviousPosition.y;

		if (orientedBox.Contains(boundingSphere) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPosition.x, dynamicPreviousPosition.y, dynamicPosition.z), false);

		// INFO: Z-axis
		if (rbBox)
			orientedBox.Center.z = dynamicPreviousPosition.z;
		else
			boundingSphere.Center.z = dynamicPreviousPosition.z;

		if (orientedBox.Contains(boundingSphere) == DirectX::DISJOINT)
			dynamicTransform->SetPosition(Vector3(dynamicPosition.x, dynamicPosition.y, dynamicPreviousPosition.z), false);
	}
}
