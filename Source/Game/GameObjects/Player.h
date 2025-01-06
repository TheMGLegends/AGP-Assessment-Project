#pragma once

#include "Core/GameObject.h"
#include "../../Observer/IObserver.h"

class Player : public GameObject, public IObserver
{
public:
	Player();
	virtual ~Player() override;

public:
	virtual void OnNotifyIsFreeCamChange(bool isFreeCam) override;

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<BoxCollider> boxCollider;
	std::weak_ptr<Rigidbody> rigidbody;
};

