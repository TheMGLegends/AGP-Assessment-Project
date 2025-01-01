#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* _gameObject, float radius);
	virtual ~SphereCollider() override;

	virtual void DrawWireframe() override;

	inline void SetRadius(float radius) { sphere.Radius = radius; }
	inline float GetRadius() const { return sphere.Radius; }

private:
	DirectX::BoundingSphere sphere;
};

