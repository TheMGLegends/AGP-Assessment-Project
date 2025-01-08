#include "Rigidbody.h"

#include "../Core/ComponentHandler.h"

#include "../Transform/Transform.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Utilities/MathUtils.h"

using namespace DirectX::SimpleMath;
using namespace MathUtils;

const Vector3 Rigidbody::GLOBAL_GRAVITY = Vector3(0.0f, -9.81f, 0.0f);
const Vector3 Rigidbody::MAX_VELOCITY = Vector3(100.0f, 100.0f, 100.0f);

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

	velocity += acceleration * deltaTime;

	velocity.x = Clamp(velocity.x, -MAX_VELOCITY.x, MAX_VELOCITY.x);
	velocity.y = Clamp(velocity.y, -MAX_VELOCITY.y, MAX_VELOCITY.y);
	velocity.z = Clamp(velocity.z, -MAX_VELOCITY.z, MAX_VELOCITY.z);

	displacement = 0.5f * acceleration * (deltaTime * deltaTime) + velocity * deltaTime;

	// INFO: Update the position of the game object
	if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
		transform->Translate(displacement);
}
