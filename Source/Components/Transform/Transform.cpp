#include "Transform.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Transform::Transform(GameObject* _gameObject) : Component(_gameObject), rotation(Quaternion::Identity), 
												scale(Vector3::One)
{
}

Transform::~Transform()
{
}

DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(position);
	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);
	XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);

	// INFO: Returns the World Matrix (World = Scale * Rotation * Translation)
	return scaleMatrix * rotationMatrix * translationMatrix;
}

DirectX::SimpleMath::Vector3 Transform::GetForwardVector() const
{
	// INFO: Get the Forward Vector based on the Rotation
	return Vector3::Transform(Vector3::Forward, rotation);
}

DirectX::SimpleMath::Vector3 Transform::GetRightVector() const
{
	// INFO: Get the Right Vector based on the Rotation
	return Vector3::Transform(Vector3::Right, rotation);
}

DirectX::SimpleMath::Vector3 Transform::GetUpVector() const
{
	// INFO: Get the Up Vector based on the Rotation
	return Vector3::Transform(Vector3::Up, rotation);
}

void Transform::Rotate(const DirectX::SimpleMath::Vector3& eulerRotation)
{
	// INFO: Create a Quaternion from the Euler Angles
	Quaternion rotationQuaternion = Quaternion::CreateFromYawPitchRoll(eulerRotation.y, eulerRotation.x, eulerRotation.z);

	// INFO: Multiply the Rotation Quaternion with the current Rotation
	rotation *= rotationQuaternion;
}

void Transform::Translate(const DirectX::SimpleMath::Vector3& translation)
{
	// INFO: Add the Translation to the Position
	position += translation;
}
