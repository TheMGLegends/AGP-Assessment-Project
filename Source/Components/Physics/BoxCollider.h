#pragma once

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	BoxCollider(GameObject* _gameObject);
	virtual ~BoxCollider() override;

	virtual void DrawWireframe() override;

private:
	DirectX::BoundingOrientedBox orientedBox;
};

