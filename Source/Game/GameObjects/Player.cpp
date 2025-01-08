#include "Player.h"

#include <DirectXCollision.h>

#include "Bullet.h"
#include "../Camera/Camera.h"
#include "../../Components/Core/ComponentHandler.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Physics/BoxCollider.h"
#include "../../Components/Physics/Rigidbody.h"
#include "../../Components/Physics/SphereCollider.h"
#include "../../Core/Input/InputHandler.h"
#include "../../Core/Time/Time.h"
#include "../../Game/GameObjects/Coin.h"
#include "../../Scene/Core/SceneContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player() : movementSpeed(10.0f), jumpStrength(15.0f), isGrounded(false), isJumping(false), jumpDuration(1.5f), 
	jumpTimer(0.0f), gun(nullptr), movementDirection(Vector3::Zero), releasedForward(true), releasedBackward(true),
	releasedLeft(true), releasedRight(true), disableGroundCheck(false)

{
	SetLayer(Layer::Player);

	mesh = AddComponent<Mesh>(this, "Cylinder", "PlayerMaterial");
	boxCollider = AddComponent<BoxCollider>(this);
	rigidbody = AddComponent<Rigidbody>(this);

	boxCollider.lock()->SetOffsetScale(Vector3(-0.8f, 1.0f, -0.8f));
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
	// INFO: Ground Check
	if (disableGroundCheck)
		disableGroundCheck = false;
	else
	{
		BoundingOrientedBox playerOrientedBox = boxCollider.lock()->GetOrientedBox();

		Ray rayCentre;
		rayCentre.position = transform.lock()->GetPosition();
		rayCentre.direction = Vector3::Down;

		rayCentre.position = XMVectorAdd(rayCentre.position, XMVectorScale(rayCentre.direction, 2.0f));

		float lowestIntersectionDistance = 100.0f;
		float intersectionDistance = 0.0f;
		for (auto& collider : ComponentHandler::GetColliders())
		{
			if (collider.lock()->GetGameObject() == boxCollider.lock()->GetGameObject())
				continue;

			if (collider.lock()->GetColliderType() == Collider::Type::Box)
			{
				if (std::shared_ptr<BoxCollider> box = std::dynamic_pointer_cast<BoxCollider>(collider.lock()))
				{
					if (box->GetOrientedBox().Intersects(rayCentre.position, rayCentre.direction, intersectionDistance) && intersectionDistance <= 0.2f)
					{
						isGrounded = true;
					}
				}
			}
			else if (collider.lock()->GetColliderType() == Collider::Type::Sphere)
			{
				if (std::shared_ptr<SphereCollider> sphere = std::dynamic_pointer_cast<SphereCollider>(collider.lock()))
				{
					if (sphere->GetSphere().Intersects(rayCentre.position, rayCentre.direction, intersectionDistance) && intersectionDistance <= 0.2f)
					{
						isGrounded = true;
					}
				}
			}

			if (lowestIntersectionDistance > intersectionDistance && intersectionDistance != 0.0f)
				lowestIntersectionDistance = intersectionDistance;
		}

		if (lowestIntersectionDistance > 0.5f)
			isGrounded = false;
	}
	

	if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
	{
		if (isGrounded)
		{
			Vector3 velocity = rb->GetVelocity();
			rb->SetUseGravity(false);
			rb->SetVelocity(Vector3(velocity.x, 0.0f, velocity.z));
		}
		else
		{
			rb->SetUseGravity(true);
		}
	}

	// INFO: Movement
	if (!releasedForward || !releasedBackward || !releasedLeft || !releasedRight)
	{
		movementDirection.Normalize();

		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			Vector3 velocity = movementDirection * movementSpeed;
			rb->SetVelocity(Vector3(velocity.x, rb->GetVelocity().y, velocity.z));
		}
	}
	else if (releasedForward && releasedBackward && releasedLeft && releasedRight)
	{
		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			Vector3 velocity = rb->GetVelocity();
			rb->SetVelocity(Vector3(0.0f, velocity.y, 0.0f));
		}
	}

	// INFO: Jumping Timer
	if (isJumping)
	{
		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			jumpTimer += deltaTime;

			if (jumpTimer >= jumpDuration)
			{
				Vector3 velocity = rb->GetVelocity();
				rb->SetVelocity(Vector3(velocity.x, 0.0f, velocity.z));

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
	// INFO: Perform a ray cast downwards to check if the player is grounded
	/*Ray ray;
	ray.position = transform.lock()->GetPosition();
	ray.direction = Vector3::Down;

	ray.position = XMVectorAdd(ray.position, XMVectorScale(ray.direction, 2.0f));

	float intersectionDistance = 0.0f;

	if (other->GetColliderType() == Collider::Type::Box)
	{
		if (std::shared_ptr<BoxCollider> box = std::dynamic_pointer_cast<BoxCollider>(other))
		{
			if (box->GetOrientedBox().Intersects(ray.position, ray.direction, intersectionDistance))
			{
				isGrounded = true;
			}
		}
	}
	else if (other->GetColliderType() == Collider::Type::Sphere)
	{
		if (std::shared_ptr<SphereCollider> sphere = std::dynamic_pointer_cast<SphereCollider>(other))
		{
			if (sphere->GetSphere().Intersects(ray.position, ray.direction, intersectionDistance))
			{
				isGrounded = true;
			}
		}
	}

	if (isGrounded)
	{
		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			Vector3 velocity = rb->GetVelocity();
			rb->SetUseGravity(false);
			rb->SetVelocity(Vector3(velocity.x, 0.0f, velocity.z));
		}
	}*/
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
		InputHandler::ClearKeyBinding(Keyboard::Keys::W, ButtonState::Held);
		InputHandler::ClearKeyBinding(Keyboard::Keys::S, ButtonState::Held);
		InputHandler::ClearKeyBinding(Keyboard::Keys::A, ButtonState::Held);
		InputHandler::ClearKeyBinding(Keyboard::Keys::D, ButtonState::Held);

		InputHandler::ClearKeyBinding(Keyboard::Keys::W, ButtonState::Released);
		InputHandler::ClearKeyBinding(Keyboard::Keys::S, ButtonState::Released);
		InputHandler::ClearKeyBinding(Keyboard::Keys::A, ButtonState::Released);
		InputHandler::ClearKeyBinding(Keyboard::Keys::D, ButtonState::Released);

		InputHandler::ClearKeyBinding(Keyboard::Keys::Space, ButtonState::Held);

		InputHandler::ClearMouseButtonBinding(MouseButton::LeftMouseButton);
	}
	// INFO: Bind if is not free cam
	else
	{
		InputHandler::BindKeyToAction(Keyboard::Keys::W, BindData(std::bind(&Player::MoveForward, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::S, BindData(std::bind(&Player::MoveBackward, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::A, BindData(std::bind(&Player::MoveLeft, this), ButtonState::Held));
		InputHandler::BindKeyToAction(Keyboard::Keys::D, BindData(std::bind(&Player::MoveRight, this), ButtonState::Held));

		InputHandler::BindKeyToAction(Keyboard::Keys::W, BindData(std::bind(&Player::ReleaseForward, this), ButtonState::Released));
		InputHandler::BindKeyToAction(Keyboard::Keys::S, BindData(std::bind(&Player::ReleaseBackward, this), ButtonState::Released));
		InputHandler::BindKeyToAction(Keyboard::Keys::A, BindData(std::bind(&Player::ReleaseLeft, this), ButtonState::Released));
		InputHandler::BindKeyToAction(Keyboard::Keys::D, BindData(std::bind(&Player::ReleaseRight, this), ButtonState::Released));

		InputHandler::BindKeyToAction(Keyboard::Keys::Space, BindData(std::bind(&Player::Jump, this), ButtonState::Held));

		InputHandler::BindMouseButtonToAction(MouseButton::LeftMouseButton, BindData(std::bind(&Player::Shoot, this), ButtonState::Pressed));
	}
}

void Player::MoveForward()
{
	float deltaTime = Time::GetDeltaTime();

	releasedForward = false;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 forward = t->GetForwardVector();
		forward.y = 0.0f;
		forward.Normalize();

		movementDirection += forward;
	}
}

void Player::MoveBackward()
{
	float deltaTime = Time::GetDeltaTime();

	releasedBackward = false;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 forward = t->GetForwardVector();
		forward.y = 0.0f;
		forward.Normalize();

		movementDirection -= forward;
	}
}

void Player::MoveLeft()
{
	float deltaTime = Time::GetDeltaTime();

	releasedLeft = false;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 right = t->GetRightVector();
		right.y = 0.0f;
		right.Normalize();

		movementDirection += right;
	}
}

void Player::MoveRight()
{
	float deltaTime = Time::GetDeltaTime();

	releasedRight = false;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 right = t->GetRightVector();
		right.y = 0.0f;
		right.Normalize();

		movementDirection -= right;
	}
}

void Player::ReleaseForward()
{
	releasedForward = true;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 forward = t->GetForwardVector();
		forward.y = 0.0f;
		forward.Normalize();

		movementDirection -= forward;
	}
}

void Player::ReleaseBackward()
{
	releasedBackward = true;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 forward = t->GetForwardVector();
		forward.y = 0.0f;
		forward.Normalize();

		movementDirection += forward;
	}
}

void Player::ReleaseLeft()
{
	releasedLeft = true;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 right = t->GetRightVector();
		right.y = 0.0f;
		right.Normalize();

		movementDirection -= right;
	}
}

void Player::ReleaseRight()
{
	releasedRight = true;

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Vector3 right = t->GetRightVector();
		right.y = 0.0f;
		right.Normalize();

		movementDirection += right;
	}
}

void Player::Jump()
{
	if (isGrounded)
	{
		if (std::shared_ptr<Rigidbody> rb = rigidbody.lock())
		{
			rb->SetUseGravity(true);
			Vector3 velocity = rb->GetVelocity();
			rb->SetVelocity(Vector3(velocity.x, jumpStrength, velocity.z));

			isJumping = true;
			disableGroundCheck = true;
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
