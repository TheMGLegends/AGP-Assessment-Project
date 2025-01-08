#include "Player.h"


#include "Bullet.h"
#include "../Camera/Camera.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Physics/BoxCollider.h"
#include "../../Components/Physics/Rigidbody.h"
#include "../../Core/Input/InputHandler.h"
#include "../../Core/Time/Time.h"
#include "../../Game/GameObjects/Coin.h"
#include "../../Scene/Core/SceneContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player() : movementSpeed(10.0f), jumpStrength(10.0f), isGrounded(false), isJumping(false), jumpDuration(1.0f), jumpTimer(0.0f), gun(nullptr)
{
	SetLayer(Layer::Player);

	mesh = AddComponent<Mesh>(this, "Cylinder", "PlayerMaterial");
	boxCollider = AddComponent<BoxCollider>(this);
	rigidbody = AddComponent<Rigidbody>(this);

	boxCollider.lock()->SetOffsetScale(Vector3(0.5f, 1.0f, 0.5f));
}

Player::~Player()
{
}

void Player::Start()
{
	// INFO: Create a gun and have it follow the player with an offset
	if (std::shared_ptr<Transform> playerTransform = transform.lock())
	{
		gun = SceneContext::SpawnGameObject<GameObject>(playerTransform->GetPosition(), Quaternion::Identity);
		gun->AddComponent<Mesh>(gun, "Gun", "GunMaterial");

		if (std::shared_ptr<Transform> gunTransform = gun->transform.lock())
		{
			gunTransform->SetScale(Vector3(3.0f, 3.0f, 3.0f));
		}
	}
}

void Player::Update(float deltaTime)
{
	if (isJumping)
	{
		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			jumpTimer += deltaTime;

			if (jumpTimer >= jumpDuration)
			{
				rb->SetVelocity(Vector3::Zero);

				jumpTimer = 0.0f;
				isJumping = false;
			}
		}
	}
}

void Player::LateUpdate(float deltaTime)
{
	// INFO: Have the gun follow the player with an offset
	if (std::shared_ptr<Transform> playerTransform = transform.lock())
	{
		if (std::shared_ptr<Transform> gunTransform = gun->transform.lock())
		{
			if (Camera* camera = SceneContext::GetScene()->GetCamera())
			{
				if (camera->GetIsFreeCam()) 
					return;

				gunTransform->SetPosition(camera->GetPosition() - camera->GetRightVector() * 0.5f + camera->GetForwardVector() * 0.75f - camera->GetUpVector() * 0.5f, false);
				
				Quaternion cameraRotation = camera->GetRotation();

				Vector3 cameraRotationEuler = cameraRotation.ToEuler();
				cameraRotationEuler.x = -cameraRotationEuler.x;

				Quaternion baseRotation = Quaternion::CreateFromYawPitchRoll(cameraRotationEuler.y, cameraRotationEuler.x, cameraRotationEuler.z);
				Quaternion newRotation = baseRotation * Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(180.0f), 0.0f, 0.0f);

				gunTransform->SetRotation(newRotation);
			}
		}
	}
}

void Player::OnCollision(std::shared_ptr<Collider> other)
{
	if (other->GetGameObject()->GetLayer() == Layer::Ground)
	{
		rigidbody.lock()->SetIsActive(false);
		isGrounded = true;
	}
}

void Player::OnTrigger(std::shared_ptr<Collider> other)
{
	if (other->GetGameObject()->GetLayer() == Layer::Coin)
	{
		if (Coin* coin = dynamic_cast<Coin*>(other->GetGameObject()))
		{
			NotifyScoreChanged(coin->GetScoreValue());
			coin->Destroy();
		}
	}
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

		InputHandler::ClearKeyBinding(Keyboard::Keys::Space);

		InputHandler::ClearMouseButtonBinding(MouseButton::LeftMouseButton);
	}
	// INFO: Bind if is not free cam
	else
	{
		InputHandler::BindKeyToAction(Keyboard::Keys::W, BindData(std::bind(&Player::MoveForward, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::S, BindData(std::bind(&Player::MoveBackward, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::A, BindData(std::bind(&Player::MoveLeft, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::D, BindData(std::bind(&Player::MoveRight, this), ButtonState::Held));

		InputHandler::BindKeyToAction(Keyboard::Keys::Space, BindData(std::bind(&Player::Jump, this), ButtonState::Held));

		InputHandler::BindMouseButtonToAction(MouseButton::LeftMouseButton, BindData(std::bind(&Player::Shoot, this), ButtonState::Pressed));
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

void Player::Jump()
{
	if (isGrounded)
	{
		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			rb->SetIsActive(true);
			rb->SetVelocity(Vector3(0.0f, jumpStrength, 0.0f));

			isJumping = true;
			isGrounded = false;
		}
	}
}

void Player::Shoot()
{
	if (gun)
	{
		if (std::shared_ptr<Transform> gunTransform = gun->transform.lock())
		{
			if (Camera* camera = SceneContext::GetScene()->GetCamera())
			{
				Vector3 spawnLocation = gunTransform->GetPosition() - gunTransform->GetForwardVector() * 0.6f + gunTransform->GetUpVector() * 0.25f;
				SceneContext::SpawnGameObject<Bullet>(spawnLocation, camera->GetRotation());
			}
		}
	}
}
