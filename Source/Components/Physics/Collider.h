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
		Sphere
	};

public:
	Collider(GameObject* _gameObject);

	virtual void DrawWireframe() {}

	inline Collider::Type GetColliderType() const { return colliderType; }

	inline void SetIsTrigger(bool _isTrigger) { isTrigger = _isTrigger; }
	inline bool GetIsTrigger() const { return isTrigger; }

	inline void ExecuteOnCollision(std::shared_ptr<Collider> other) { OnCollision(other); }
	inline void ExecuteOnTrigger(std::shared_ptr<Collider> other) { OnTrigger(other); }

protected:
	Collider::Type colliderType;

private:
	bool isTrigger;

	std::function<void(std::shared_ptr<Collider>)> OnCollision;
	std::function<void(std::shared_ptr<Collider>)> OnTrigger;
};

