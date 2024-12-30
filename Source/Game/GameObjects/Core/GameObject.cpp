#include "GameObject.h"

using namespace DirectX;

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

XMMATRIX GameObject::LookAtXZ(float xTarget, float zTarget) const
{
	XMFLOAT3 position = transform.lock()->GetPosition();

	// INFO: Calculate the direction vector for the x and z axis
	float deltaX = xTarget - position.x;
	float deltaZ = zTarget - position.z;

	// INFO: Calculate the angle in radians
	float angle = -atan2f(deltaZ, deltaX);

	// INFO: Create the rotation matrix around the y-axis
	return XMMatrixRotationY(angle - XMConvertToRadians(90.0f));
}
