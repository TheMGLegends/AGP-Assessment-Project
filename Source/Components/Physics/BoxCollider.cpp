#include "BoxCollider.h"

#include "../../Game/GameObjects/Core/GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

BoxCollider::BoxCollider(GameObject* _gameObject) : Collider(_gameObject), offsetScale(Vector3::Zero)
{
	colliderType = Type::Box;

	// INFO: Initialise the orientedBox
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
	{
		orientedBox.Center = transform->GetPosition();
		orientedBox.Extents = transform->GetScale() + offsetScale;
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
		orientedBox.Extents = transform->GetScale() + offsetScale;
		orientedBox.Orientation = transform->GetRotation();
	}
}

void BoxCollider::DrawWireframe(ID3D11DeviceContext* deviceContext)
{
	static const XMVECTOR vertices[8] =
	{
		{ -1, -1, -1, 0 },
		{ 1, -1, -1, 0 },
		{ 1, -1, 1, 0 },
		{ -1, -1, 1, 0 },
		{ -1, 1, -1, 0 },
		{ 1, 1, -1, 0 },
		{ 1, 1, 1, 0 },
		{ -1, 1, 1, 0 }
	};

	static const unsigned short indices[24] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	std::shared_ptr<Transform> owningTransform = GetGameObject()->transform.lock();

	if (owningTransform)
	{
		XMMATRIX worldMatrix = owningTransform->GetWorldMatrix(offsetScale);

		// INFO: Translate the vertices to world space
		VertexPositionColor worldVertices[8]{};

		for (size_t i = 0; i < 8; i++)
		{
			XMStoreFloat3(&worldVertices[i].position, XMVector3Transform(vertices[i], worldMatrix));
			XMStoreFloat4(&worldVertices[i].color, wireframeColour);
		}

		batchEffect->Apply(deviceContext);

		deviceContext->IASetInputLayout(batchInputLayout.Get());

		primitiveBatch->Begin();

		primitiveBatch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, indices, 24, worldVertices, 8);

		primitiveBatch->End();
	}
}
