#include "Collider.h"

#include "../../Game/GameObjects/Core/GameObject.h"

Collider::Collider(GameObject* _gameObject) : Component(_gameObject), colliderType(Type::None), isTrigger(false)
{
	// INFO: Bind callbacks to the game object functions
	if (GameObject* gameObject = GetGameObject())
	{
		OnCollision = std::bind(&GameObject::OnCollision, gameObject, std::placeholders::_1);
		OnTrigger = std::bind(&GameObject::OnTrigger, gameObject, std::placeholders::_1);
	}
}
