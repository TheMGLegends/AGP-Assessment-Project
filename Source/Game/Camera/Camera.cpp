#include "Camera.h"

#include "../../Components/Transform/Transform.h"

using namespace DirectX::SimpleMath;

Camera::Camera() : position(Vector3::Zero), offset(Vector3::Zero),
				   rotation(Quaternion::CreateFromYawPitchRoll(0.0f, DirectX::XM_PIDIV2, DirectX::XM_PI)),
				   target(nullptr), freeCamInfo(FreeCamInfo()), projectionInfo(ProjectionInfo())
{
}

Camera::Camera(const Vector3& _position, const Vector3& _offset, const Quaternion& _rotation, const FreeCamInfo& _freeCamInfo,
			   const ProjectionInfo& _projectionInfo, Transform* _target) : position(_position), offset(_offset), rotation(_rotation),
																			freeCamInfo(_freeCamInfo), projectionInfo(_projectionInfo),
																			target(_target)
{
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR lookTo = GetForwardVector();
	DirectX::XMVECTOR up = GetUpVector();

	return DirectX::XMMatrixLookAtLH(eye, lookTo, up);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(projectionInfo.fovAngleY), 
											 projectionInfo.aspectRatio, projectionInfo.nearZ, projectionInfo.farZ);
}

Vector3 Camera::GetForwardVector() const
{
	return Vector3::Transform(Vector3::Forward, rotation);
}

Vector3 Camera::GetRightVector() const
{
	return Vector3::Transform(Vector3::Right, rotation);
}

Vector3 Camera::GetUpVector() const
{
	return Vector3::Transform(Vector3::Up, rotation);
}

void Camera::Update(float deltaTime)
{
	// INFO: Different update behavior based on if camera is free or not
	if (freeCamInfo.isFreeCam)
	{
		// TODO: Implement Free Camera Update Logic
	}
	else if (target != nullptr)
	{
		position += target->GetPosition() + offset;
	}
}

void Camera::Reset()
{
	if (!freeCamInfo.isFreeCam)
		return;

	position = Vector3::Zero;
	rotation = Quaternion::CreateFromYawPitchRoll(0.0f, DirectX::XM_PIDIV2, 0.0f);
}
