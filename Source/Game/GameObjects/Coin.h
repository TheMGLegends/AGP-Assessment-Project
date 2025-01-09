#pragma once

#include "Core/GameObject.h"

class Coin : public GameObject
{
public:
	Coin();
	virtual ~Coin() override;

	virtual void Update(float deltaTime) override;
	virtual void OnDestroy() override;

	inline int GetScoreValue() const { return scoreValue; }

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<SphereCollider> sphereCollider;

	int scoreValue;

	float rotationSpeed;
	float travelDistance;
};

