#pragma once

#include <SimpleMath.h>

class GameObject;

class Component
{
	friend class GameObject;

public:
	Component(GameObject* _gameObject);
	virtual ~Component() = 0;

	virtual void Update(float deltaTime) {}

	inline GameObject* GetGameObject() const { return gameObject; }
	inline unsigned int GetComponentID() const { return componentID; }

	/// @param updatePreviousPosition: If true, the previous position will be updated to the current position
	void SetPosition(const DirectX::SimpleMath::Vector3& _position, bool updatePreviousPosition = true);
	inline const DirectX::SimpleMath::Vector3& GetPosition() const { return position; }

	inline const DirectX::SimpleMath::Vector3& GetPreviousPosition() const { return previousPosition; }

	inline void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { offset = _offset; }
	inline const DirectX::SimpleMath::Vector3& GetOffset() const { return offset; }

	inline void SetIsActive(bool _isActive) { isActive = _isActive; }
	inline bool GetIsActive() const { return isActive; }

	inline bool GetCanHaveMultiple() const { return canHaveMultiple; }

protected:
	/// @brief Sets the previous position to the current position
	inline void SetPreviousPosition() { previousPosition = position; }

protected:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 previousPosition;
	DirectX::SimpleMath::Vector3 offset;

	bool isActive;
	bool canHaveMultiple;

private:
	GameObject* gameObject;

	unsigned int componentID;
	static unsigned int componentCount;
};

