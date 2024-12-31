#pragma once

#include "../Core/Component.h"

#include <memory>

class Rigidbody : public Component, public std::enable_shared_from_this<Rigidbody>
{
public:
	Rigidbody(GameObject* _gameObject);
	virtual ~Rigidbody() override;

	virtual void Update(float deltaTime) override;

	inline void SetGravity(const DirectX::SimpleMath::Vector3& _gravity) { gravity = _gravity; }
	inline const DirectX::SimpleMath::Vector3& GetGravity() const { return gravity; }

	inline void SetVelocity(const DirectX::SimpleMath::Vector3& _velocity) { velocity = _velocity; }
	inline const DirectX::SimpleMath::Vector3& GetVelocity() const { return velocity; }

	inline void SetUseGravity(bool _useGravity) { useGravity = _useGravity; }
	inline bool GetUseGravity() const { return useGravity; }

private:
	virtual void RegisterComponent() override;

private:
	static const DirectX::SimpleMath::Vector3 GLOBAL_GRAVITY;

	DirectX::SimpleMath::Vector3 gravity;
	DirectX::SimpleMath::Vector3 velocity;
	DirectX::SimpleMath::Vector3 acceleration;

	bool useGravity;
};

