#pragma once

#include "Core/GameObject.h"
#include "../../Observer/IObserver.h"
#include "../../Observer/Subject.h"

class Player : public GameObject, public IObserver, public Subject
{
public:
	Player();
	virtual ~Player() override;

	virtual void Update(float deltaTime) override;
	virtual void OnCollision(std::shared_ptr<Collider> other) override;
	virtual void OnTrigger(std::shared_ptr<Collider> other) override;

	inline void SetMovementSpeed(float _movementSpeed) { movementSpeed = _movementSpeed; }
	inline float GetMovementSpeed() const { return movementSpeed; }

public:
	virtual void OnNotifyIsFreeCamChange(bool isFreeCam) override;

private:
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	void Jump();

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<BoxCollider> boxCollider;
	std::weak_ptr<Rigidbody> rigidbody;

	float movementSpeed;
	float jumpStrength;

	bool isGrounded;
	bool isJumping;

	float jumpDuration;
	float jumpTimer;
};

