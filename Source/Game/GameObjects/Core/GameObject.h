#pragma once

#include <DirectXMath.h>
#include <memory>
#include <vector>

//#include "../../../Components/Physics/Collider.h"
#include "../../../Components/Transform/Transform.h"

class Component;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = 0;

	template<class T>
	std::weak_ptr<T> GetComponent();

	template<class T, typename... Args>
	std::weak_ptr<T> AddComponent(Args&&... args);

	template<class T>
	bool RemoveComponent(unsigned int componentID);

	inline void SetIsActive(bool _isActive) { isActive = _isActive; }
	inline bool GetIsActive() const { return isActive; }

	void Destroy();
	inline bool GetIsMarkedForDeletion() const { return markedForDeletion; }

	DirectX::XMMATRIX LookAtXZ(float xTarget, float zTarget) const;

	virtual void Start() {}
	virtual void Update(float deltaTime) {}
	virtual void LateUpdate(float deltaTime) {}

	//virtual void OnCollision(std::shared_ptr<Collider> other) {} // TODO: This uncommented once collider is done
	virtual void OnDestroy() {}

public:
	std::weak_ptr<Transform> transform;

protected:
	bool isActive;

private:
	std::vector<std::shared_ptr<Component>> components;

	bool markedForDeletion;
};

template<class T>
inline std::weak_ptr<T> GameObject::GetComponent()
{
	// INFO: Check if component exists
	for (auto& component : components)
	{
		std::weak_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);

		if (!castedComponent.expired())
			return castedComponent;
	}

	return std::weak_ptr<T>();
}

template<class T, typename ...Args>
inline std::weak_ptr<T> GameObject::AddComponent(Args && ...args)
{
	// INFO: Ensure T is a derived class of Component
	static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

	// INFO: Check if component already exists
	std::weak_ptr<T> existingComponent = GetComponent<T>();

	// INFO: Do not add the component if it already exists and we can only have one
	if (!existingComponent.expired() && !existingComponent.lock()->GetCanHaveMultiple())
		return existingComponent;

	components.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));

	// INFO: Register the component with the ComponentHandler
	components.back()->RegisterComponent();

	return std::dynamic_pointer_cast<T>(components.back());
}

template<class T>
inline bool GameObject::RemoveComponent(unsigned int componentID)
{
	// INFO: Compare component IDs to find the component to remove
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		T* castedComponent = dynamic_cast<T*>(it->get());

		if (castedComponent && castedComponent->GetComponentID() == componentID)
		{
			components.erase(it);
			return true;
		}
	}

	return false;
}
