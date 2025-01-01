#pragma once

#include "Core/GameObject.h"

class Player : public GameObject
{
public:
	Player();
	virtual ~Player() override;

	virtual void OnCollision(std::shared_ptr<Collider> other) override;

private:

};

