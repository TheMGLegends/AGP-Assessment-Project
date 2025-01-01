#pragma once

#include "../Core/Component.h"

#include <functional>
#include <memory>

class Collider : public Component
{
public:
	enum class Type
	{
		None = 0,

		Box,
		Circle
	};

public:
	Collider(GameObject* _gameObject);

	virtual void DrawWireframe() {}

	inline Collider::Type GetColliderType() const { return colliderType; }

protected:
	Collider::Type colliderType;

private:
	std::function<void(std::shared_ptr<Collider>)> OnCollision;
};

