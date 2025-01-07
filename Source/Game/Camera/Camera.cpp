#include "Camera.h"

#include <iostream>

#include "../../Components/Transform/Transform.h"
#include "../../Core/Input/InputHandler.h"
#include "../../Utilities/MathUtils.h"

using namespace DirectX::SimpleMath;
using namespace MathUtils;

Camera::Camera() : position(Vector3(0.0f, 15.0f, 0.0f)), offset(Vector3::Zero),
				   rotation(Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0.0f, 0.0f)),
				   pitchConstraints(-1.49f, 1.49f), target(nullptr), freeCamInfo(FreeCamInfo()), projectionInfo(ProjectionInfo())
{
	InputHandler::BindKeyToAction(DirectX::Keyboard::Keys::R, BindData(std::bind(&Camera::Reset, this), ButtonState::Pressed));
}

Camera::Camera(const Vector3& _position, const Vector3& _offset, const Quaternion& _rotation, const Vector2& _pitchConstraints, 
			   const FreeCamInfo& _freeCamInfo, const ProjectionInfo& _projectionInfo, Transform* _target) : position(_position), offset(_offset), 
																										     rotation(_rotation), pitchConstraints(_pitchConstraints),
																											 freeCamInfo(_freeCamInfo), projectionInfo(_projectionInfo),
																											 target(_target)
{
	InputHandler::BindKeyToAction(DirectX::Keyboard::Keys::R, BindData(std::bind(&Camera::Reset, this), ButtonState::Pressed));
}

Camera::~Camera()
{
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR lookTo = DirectX::XMVectorAdd(eye, GetForwardVector());
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
	// INFO: Rotation
	if (InputHandler::GetMouseMode() == DirectX::Mouse::Mode::MODE_RELATIVE)
	{
		DirectX::XMINT2 mouseInput = InputHandler::GetMousePosition();

		std::cout << "Mouse Input: X: " << mouseInput.x << " Y: " << mouseInput.y << "\n";

		// INFO: Euler rotation in radians
		Vector3 eulerRotation = rotation.ToEuler();

		eulerRotation.y += mouseInput.x * freeCamInfo.rotationSpeed;
		eulerRotation.x -= mouseInput.y * freeCamInfo.rotationSpeed;

		eulerRotation.x = Clamp(eulerRotation.x, pitchConstraints.x, pitchConstraints.y);

		rotation = Quaternion::CreateFromYawPitchRoll(eulerRotation.y, eulerRotation.x, 0.0f);
	}

	// INFO: Different update behavior based on if camera is free or not
	if (freeCamInfo.isFreeCam)
	{
		// INFO: Movement
		if (InputHandler::GetKey(DirectX::Keyboard::Keys::W))
			position += GetForwardVector() * freeCamInfo.movementSpeed * deltaTime;

		if (InputHandler::GetKey(DirectX::Keyboard::Keys::S))
			position -= GetForwardVector() * freeCamInfo.movementSpeed * deltaTime;

		if (InputHandler::GetKey(DirectX::Keyboard::Keys::A))
			position += GetRightVector() * freeCamInfo.movementSpeed * deltaTime;

		if (InputHandler::GetKey(DirectX::Keyboard::Keys::D))
			position -= GetRightVector() * freeCamInfo.movementSpeed * deltaTime;
	}
	else if (target != nullptr)
	{
		// INFO: Adjust target yaw rotation based on camera yaw
		Vector3 cameraEuler = rotation.ToEuler();
		Vector3 targetEuler = target->GetRotation().ToEuler();

		targetEuler.y = cameraEuler.y;

		target->SetRotation(Quaternion::CreateFromYawPitchRoll(targetEuler.y, targetEuler.x, targetEuler.z));

		position = target->GetPosition() + offset;
	}
}

void Camera::Reset()
{
	if (!freeCamInfo.isFreeCam)
		return;

	position = Vector3(0.0f, 15.0f, 0.0f);
	rotation = Quaternion::CreateFromYawPitchRoll(DirectX::XM_PI, 0.0f, 0.0f);
}

void Camera::SetIsFreeCam(bool _isFreeCam)
{
	freeCamInfo.isFreeCam = _isFreeCam;

	// INFO: Notify observers of free cam change
	NotifyIsFreeCamChanged(freeCamInfo.isFreeCam);
}
