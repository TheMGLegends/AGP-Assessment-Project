#include "GameObject.h"

#include "../../../Components/Transform/Transform.h"

GameObject::GameObject() : isActive(true), markedForDeletion(false)
{
	transform = AddComponent<Transform>(this);
}

GameObject::~GameObject()
{
}

void GameObject::Destroy()
{
	// INFO: Return if the GameObject is already marked for Deletion
	if (markedForDeletion)
		return;

	// INFO: Call the OnDestroy Event
	OnDestroy();

	// INFO: Mark the GameObject for deletion and deactivate it
	markedForDeletion = true;
	isActive = false;
}
