#include "SphereCollider.h"

#include "../../Game/GameObjects/Core/GameObject.h"

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
	DirectX::XMVECTOR centre = XMLoadFloat3(&sphere.Center);
	float radius = sphere.Radius;

	DirectX::XMVECTOR xRange = DirectX::XMVectorSet(radius, 0, 0, 0);
	DirectX::XMVECTOR yRange = DirectX::XMVectorSet(0, radius, 0, 0);
	DirectX::XMVECTOR zRange = DirectX::XMVectorSet(0, 0, radius, 0);

	DrawRing(deviceContext, centre, xRange, yRange);
	DrawRing(deviceContext, centre, xRange, zRange);
	DrawRing(deviceContext, centre, yRange, zRange);
}

void SphereCollider::DrawRing(ID3D11DeviceContext* deviceContext, const DirectX::XMVECTOR& centre, const DirectX::XMVECTOR& majorAxis, const DirectX::XMVECTOR& minorAxis)
{
	static const int ringSegments = 32;

	DirectX::VertexPositionColor vertices[ringSegments + 1]{};

	float segmentAngle = DirectX::XM_2PI / static_cast<float>(ringSegments);
	DirectX::XMVECTOR cosSegment = DirectX::XMVectorReplicate(cosf(segmentAngle));
	DirectX::XMVECTOR sinSegment = DirectX::XMVectorReplicate(sinf(segmentAngle));
	DirectX::XMVECTOR incrementalSin = DirectX::XMVectorZero();

	static const DirectX::XMVECTOR initialCos = { 1.0f, 1.0f, 1.0f, 1.0f };

	DirectX::XMVECTOR incrementalCos = initialCos;

	for (size_t i = 0; i < ringSegments; i++)
	{
		DirectX::XMVECTOR position{};

		position = DirectX::XMVectorMultiplyAdd(majorAxis, incrementalCos, centre);
		position = DirectX::XMVectorMultiplyAdd(minorAxis, incrementalSin, position);

		XMStoreFloat3(&vertices[i].position, position);
		XMStoreFloat4(&vertices[i].color, wireframeColour);

		DirectX::XMVECTOR newCos = DirectX::XMVectorSubtract(DirectX::XMVectorMultiply(incrementalCos, cosSegment), DirectX::XMVectorMultiply(incrementalSin, sinSegment));
		DirectX::XMVECTOR newSin = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(incrementalCos, sinSegment), DirectX::XMVectorMultiply(incrementalSin, cosSegment));

		incrementalCos = newCos;
		incrementalSin = newSin;
	}

	vertices[ringSegments] = vertices[0];

	batchEffect->Apply(deviceContext);

	deviceContext->IASetInputLayout(batchInputLayout.Get());

	primitiveBatch->Begin();

	primitiveBatch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, vertices, static_cast<size_t>(ringSegments) + 1);

	primitiveBatch->End();
}
