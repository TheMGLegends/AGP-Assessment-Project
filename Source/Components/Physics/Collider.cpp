#include "Collider.h"

Collider::Collider(GameObject* _gameObject) : Component(_gameObject), colliderType(Type::None), OnCollision(nullptr)
{
}
