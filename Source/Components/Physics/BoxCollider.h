#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* _gameObject);
	virtual ~BoxCollider() override;

	virtual void Update(float deltaTime) override;
	virtual void DrawWireframe(ID3D11DeviceContext* deviceContext) override;

	inline DirectX::BoundingOrientedBox& GetOrientedBox() { return orientedBox; }

private:
	DirectX::BoundingOrientedBox orientedBox;
};

