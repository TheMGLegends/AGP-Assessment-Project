#include "SphereCollider.h"

#include "../../Game/GameObjects/Core/GameObject.h"

using namespace DirectX;

SphereCollider::SphereCollider(GameObject* _gameObject, float radius) : Collider(_gameObject)
{
	colliderType = Type::Circle;

	// INFO: Initialise the sphere
	if (auto transform = GetGameObject()->transform.lock())
	{
		sphere.Center = transform->GetPosition();
		sphere.Radius = radius;
	}
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::DrawWireframe()
{
	// TODO: Setup DirectXCollision PrimitiveBatch to draw the wire frame of the sphere
}
