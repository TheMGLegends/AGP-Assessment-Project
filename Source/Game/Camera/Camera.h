#pragma once

#include <SimpleMath.h>

class Transform;

struct FreeCamInfo
{
	bool isFreeCam;
	float speed;

	FreeCamInfo() : isFreeCam(false), speed(1.0f) {};
	FreeCamInfo(bool _isFreeCam, float _speed) : isFreeCam(_isFreeCam), 
												 speed(_speed) {};
};

struct ProjectionInfo
{
	float fovAngleY;
	float aspectRatio;
	float nearZ;
	float farZ;

	ProjectionInfo() : fovAngleY(60), aspectRatio(16.0f / 9.0f), 
					   nearZ(0.1f), farZ(100.0f) {};

	/// @param _fovAngleY : The field of view angle in the y-direction, in degrees
	/// @param width : The width of the viewport
	/// @param height : The height of the viewport
	ProjectionInfo(float _fovAngleY, float width, float height, float _nearZ, float _farZ) 
		: fovAngleY(_fovAngleY), aspectRatio(width / height), nearZ(_nearZ), farZ(_farZ) {};
};

class Camera
{
public:
	Camera();
	Camera(const DirectX::SimpleMath::Vector3& _position, const DirectX::SimpleMath::Vector3& _offset, 
		   const DirectX::SimpleMath::Quaternion& _rotation, const FreeCamInfo& _freeCamInfo = FreeCamInfo(), 
		   const ProjectionInfo& _projectionInfo = ProjectionInfo(), Transform* _target = nullptr);
	~Camera();

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

	DirectX::SimpleMath::Vector3 GetForwardVector() const;
	DirectX::SimpleMath::Vector3 GetRightVector() const;
	DirectX::SimpleMath::Vector3 GetUpVector() const;

	void Update(float deltaTime);

	/// @brief Resets the camera to its default position and rotation, doesn't reset the other settings
	void Reset();

	inline const DirectX::SimpleMath::Vector3& GetPosition() const { return position; }
	inline void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { offset = _offset; }

	/// @brief Setting target automatically sets camera to not be free
	inline void SetTarget(Transform* _target) { target = _target; freeCamInfo.isFreeCam = false; }

	/// @return The free cam info of the camera, use this to modify the free cam settings
	FreeCamInfo& GetFreeCamInfo() { return freeCamInfo; }

	/// @return The projection info of the camera, use this to modify the projection settings
	ProjectionInfo& GetProjectionInfo() { return projectionInfo; }

private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 offset;
	DirectX::SimpleMath::Quaternion rotation;

	Transform* target;

	FreeCamInfo freeCamInfo;
	ProjectionInfo projectionInfo;
};

