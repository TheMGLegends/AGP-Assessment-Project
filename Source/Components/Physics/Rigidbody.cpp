#include "Rigidbody.h"

#include "../Core/ComponentHandler.h"

#include "../Transform/Transform.h"
#include "../../Game/GameObjects/Core/GameObject.h"

using namespace DirectX::SimpleMath;

const Vector3 Rigidbody::GLOBAL_GRAVITY = Vector3(0.0f, -500.0f, 0.0f);

Rigidbody::Rigidbody(GameObject* _gameObject) : Component(_gameObject), gravity(GLOBAL_GRAVITY), velocity(Vector3::Zero), 
												acceleration(Vector3::Zero), useGravity(true)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Update(float deltaTime)
{
	acceleration = useGravity ? gravity : Vector3::Zero;
	Vector3 displacement = 0.5f * acceleration * (deltaTime * deltaTime) + velocity * deltaTime;

	// INFO: Update the position of the game object
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
		transform->Translate(displacement);
}
