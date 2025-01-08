#pragma once

#include "../Core/Component.h"

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* _gameObject);
	virtual ~Rigidbody() override;

	virtual void Update(float deltaTime) override;

	inline void SetGravity(const DirectX::SimpleMath::Vector3& _gravity) { gravity = _gravity; }
	inline const DirectX::SimpleMath::Vector3& GetGravity() const { return gravity; }

	inline void SetVelocity(const DirectX::SimpleMath::Vector3& _velocity) { velocity = _velocity; }
	inline const DirectX::SimpleMath::Vector3& GetVelocity() const { return velocity; }

	inline const DirectX::SimpleMath::Vector3& GetDisplacement() const { return displacement; }

	inline void SetUseGravity(bool _useGravity) { useGravity = _useGravity; }
	inline bool GetUseGravity() const { return useGravity; }

private:
	static const DirectX::SimpleMath::Vector3 GLOBAL_GRAVITY;
	static const DirectX::SimpleMath::Vector3 MAX_VELOCITY;

	DirectX::SimpleMath::Vector3 gravity;
	DirectX::SimpleMath::Vector3 velocity;
	DirectX::SimpleMath::Vector3 acceleration;
	DirectX::SimpleMath::Vector3 displacement;

	bool useGravity;
};

