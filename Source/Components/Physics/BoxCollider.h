#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* _gameObject);
	virtual ~BoxCollider() override;

	virtual void Update(float deltaTime) override;
	virtual void DrawWireframe() override;

	inline DirectX::BoundingOrientedBox& GetOrientedBox() { return orientedBox; }

private:
	DirectX::BoundingOrientedBox orientedBox;
};

