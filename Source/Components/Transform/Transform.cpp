#include "Transform.h"

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
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslationFromVector(position);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);

	// INFO: Returns the World Matrix (World = Scale * Rotation * Translation)
	return scaleMatrix * rotationMatrix * translationMatrix;
}

Vector3 Transform::GetForwardVector() const
{
	return Vector3::Transform(Vector3::Forward, rotation);
}

Vector3 Transform::GetRightVector() const
{
	return Vector3::Transform(Vector3::Right, rotation);
}

Vector3 Transform::GetUpVector() const
{
	return Vector3::Transform(Vector3::Up, rotation);
}

void Transform::Rotate(const Vector3& eulerRotation)
{
	Quaternion rotationQuaternion = Quaternion::CreateFromYawPitchRoll(eulerRotation.y, eulerRotation.x, eulerRotation.z);

	// INFO: Combine the current rotation with the new rotation
	rotation *= rotationQuaternion;
}

