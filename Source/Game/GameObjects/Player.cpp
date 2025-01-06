#include "Player.h"

#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Physics/BoxCollider.h"
#include "../../Components/Physics/Rigidbody.h"
#include "../../Core/Input/InputHandler.h"
#include "../../Core/Time/Time.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player() : movementSpeed(5.0f)
{
	SetLayer(Layer::Player);

	mesh = AddComponent<Mesh>(this, "Cylinder", "PlayerMaterial");
	boxCollider = AddComponent<BoxCollider>(this);
	rigidbody = AddComponent<Rigidbody>(this);

	boxCollider.lock()->SetOffsetScale(Vector3(0.5f, 1.0f, 0.5f));
	//rigidbody.lock()->SetIsActive(false);
}

Player::~Player()
{
}

void Player::OnNotifyIsFreeCamChange(bool isFreeCam)
{
	// INFO: Unbind if is free cam
	if (isFreeCam)
	{
		InputHandler::ClearKeyBinding(Keyboard::Keys::W);
		InputHandler::ClearKeyBinding(Keyboard::Keys::S);
		InputHandler::ClearKeyBinding(Keyboard::Keys::A);
		InputHandler::ClearKeyBinding(Keyboard::Keys::D);
	}
	// INFO: Bind if is not free cam
	else
	{
		InputHandler::BindKeyToAction(Keyboard::Keys::W, BindData(std::bind(&Player::MoveForward, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::S, BindData(std::bind(&Player::MoveBackward, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::A, BindData(std::bind(&Player::MoveLeft, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::D, BindData(std::bind(&Player::MoveRight, this), ButtonState::Held));
	}
}

void Player::MoveForward()
{
	float deltaTime = Time::GetDeltaTime();

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 forward = t->GetForwardVector();
		forward.y = 0.0f;

		t->Translate(forward * movementSpeed * deltaTime);
	}
}

void Player::MoveBackward()
{
	float deltaTime = Time::GetDeltaTime();

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 forward = t->GetForwardVector();
		forward.y = 0.0f;

		t->Translate(-forward * movementSpeed * deltaTime);
	}
}

void Player::MoveLeft()
{
	float deltaTime = Time::GetDeltaTime();

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 right = t->GetRightVector();
		right.y = 0.0f;

		t->Translate(right * movementSpeed * deltaTime);
	}
}

void Player::MoveRight()
{
	float deltaTime = Time::GetDeltaTime();

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 right = t->GetRightVector();
		right.y = 0.0f;

		t->Translate(-right * movementSpeed * deltaTime);
	}
}
