#include "SphereCollider.h"

#include "../../Game/GameObjects/Core/GameObject.h"

using namespace DirectX;

SphereCollider::SphereCollider(GameObject* _gameObject, float radius) : Collider(_gameObject)
{
	colliderType = Type::Sphere;

	// INFO: Initialise the sphere
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
	{
		sphere.Center = transform->GetPosition();
		sphere.Radius = radius;
	}
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update(float deltaTime)
{
	// INFO: Match the sphere with the transform of the game object
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
	{
		sphere.Center = transform->GetPosition();
	}
}

void SphereCollider::DrawWireframe(ID3D11DeviceContext* deviceContext)
{
	// TODO: Setup DirectXCollision PrimitiveBatch to draw the wire frame of the sphere
}
