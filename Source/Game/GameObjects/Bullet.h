#pragma once

#include "Core/GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	virtual ~Bullet() override;

	virtual void Update(float deltaTime) override;
	virtual void OnTrigger(std::shared_ptr<Collider> other) override;

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<SphereCollider> sphereCollider;

	float movementSpeed;
	float lifetimeDuration;
	float lifetimeTimer;
};

