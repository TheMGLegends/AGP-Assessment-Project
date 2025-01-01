#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* _gameObject, float radius);
	virtual ~SphereCollider() override;

	virtual void Update(float deltaTime) override;
	virtual void DrawWireframe() override;

	inline void SetRadius(float radius) { sphere.Radius = radius; }
	inline float GetRadius() const { return sphere.Radius; }

	inline DirectX::BoundingSphere& GetSphere() { return sphere; }

private:
	DirectX::BoundingSphere sphere;
};

