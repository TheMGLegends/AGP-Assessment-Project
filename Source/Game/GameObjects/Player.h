#pragma once

#include "Core/GameObject.h"
#include "../../Observer/IObserver.h"

class Player : public GameObject, public IObserver
{
public:
	Player();
	virtual ~Player() override;

	inline void SetMovementSpeed(float _movementSpeed) { movementSpeed = _movementSpeed; }
	inline float GetMovementSpeed() const { return movementSpeed; }

public:
	virtual void OnNotifyIsFreeCamChange(bool isFreeCam) override;

private:
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<BoxCollider> boxCollider;
	std::weak_ptr<Rigidbody> rigidbody;

	float movementSpeed;
};

