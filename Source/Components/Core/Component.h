#pragma once

#include <SimpleMath.h>

class GameObject;

class Component
{
public:
	Component(GameObject* _gameObject);
	virtual ~Component() = 0;

	virtual void Update(float deltaTime) {}

	inline GameObject* GetGameObject() const { return gameObject; }
	inline unsigned int GetComponentID() const { return componentID; }

	inline void SetPosition(const DirectX::SimpleMath::Vector3& _position) { position = _position; }
	inline DirectX::SimpleMath::Vector3 GetPosition() const { return position; }

	inline void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { offset = _offset; }
	inline DirectX::SimpleMath::Vector3 GetOffset() const { return offset; }

	inline void SetIsActive(bool _isActive) { isActive = _isActive; }
	inline bool GetIsActive() const { return isActive; }

	inline bool GetCanHaveMultiple() const { return canHaveMultiple; }

protected:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 offset;

	bool isActive;
	bool canHaveMultiple;

private:
	GameObject* gameObject;

	unsigned int componentID;
	static unsigned int componentCount;
};

