#include "BoxCollider.h"

#include <VertexTypes.h>

#include "../../Game/GameObjects/Core/GameObject.h"

using namespace DirectX;

BoxCollider::BoxCollider(GameObject* _gameObject) : Collider(_gameObject)
{
	colliderType = Type::Box;

	// INFO: Initialise the orientedBox
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
	{
		orientedBox.Center = transform->GetPosition();
		orientedBox.Extents = transform->GetScale(); // TODO: Maybe / 2.0f?
		orientedBox.Orientation = transform->GetRotation();
	}
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Update(float deltaTime)
{
	// INFO: Match the orientedBox with the transform of the game object
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
	{
		orientedBox.Center = transform->GetPosition();
		orientedBox.Extents = transform->GetScale(); // TODO: Maybe / 2.0f?
		orientedBox.Orientation = transform->GetRotation();
	}
}

void BoxCollider::DrawWireframe(ID3D11DeviceContext* deviceContext)
{
	// TODO: Setup DirectXCollision PrimitiveBatch to draw the wire frame of the orientedBox
}
