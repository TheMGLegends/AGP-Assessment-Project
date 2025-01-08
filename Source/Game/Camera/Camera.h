#pragma once

#include "../../Observer/Subject.h"

#include <SimpleMath.h>

class Transform;

struct FreeCamInfo
{
	bool isFreeCam;
	float movementSpeed;
	float rotationSpeed;

	FreeCamInfo() : isFreeCam(true), movementSpeed(10.0f), rotationSpeed(0.001f) {};
	FreeCamInfo(bool _isFreeCam, float _movementSpeed, float _rotationSpeed) : isFreeCam(_isFreeCam), 
																			   movementSpeed(_movementSpeed), 
																			   rotationSpeed(_rotationSpeed) {};
};

struct ProjectionInfo
{
	float fovAngleY;
	float aspectRatio;
	float nearZ;
	float farZ;

	ProjectionInfo() : fovAngleY(90.0f), aspectRatio(16.0f / 9.0f), 
					   nearZ(0.1f), farZ(100.0f) {};

	/// @param _fovAngleY : In degrees
	/// @param width : Viewport width
	/// @param height : Viewport height
	ProjectionInfo(float _fovAngleY, float width, float height, float _nearZ, float _farZ) 
		: fovAngleY(_fovAngleY), aspectRatio(width / height), nearZ(_nearZ), farZ(_farZ) {};
};

class Camera : public Subject
{
public:
	Camera();
	Camera(const DirectX::SimpleMath::Vector3& _position, const DirectX::SimpleMath::Vector3& _offset, 
		   const DirectX::SimpleMath::Quaternion& _rotation, const DirectX::SimpleMath::Vector2& _pitchConstraints, 
		   const FreeCamInfo& _freeCamInfo = FreeCamInfo(), const ProjectionInfo& _projectionInfo = ProjectionInfo(), 
		   Transform* _target = nullptr);
	~Camera();

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjectionMatrix() const;

	DirectX::SimpleMath::Vector3 GetForwardVector() const;
	DirectX::SimpleMath::Vector3 GetRightVector() const;
	DirectX::SimpleMath::Vector3 GetUpVector() const;

	void LateUpdate(float deltaTime);

	/// @brief Only resets cameras position and rotation if camera is free
	void Reset();

	inline const DirectX::SimpleMath::Vector3& GetPosition() const { return position; }
	inline void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { offset = _offset; }

	inline const DirectX::SimpleMath::Quaternion& GetRotation() const { return rotation; }

	inline void SetPitchConstraints(float minPitch, float maxPitch) { pitchConstraints = DirectX::SimpleMath::Vector2(minPitch, maxPitch); }
	inline const DirectX::SimpleMath::Vector2& GetPitchConstraints() const { return pitchConstraints; }

	inline void SetTarget(Transform* _target) { target = _target; }

	void SetIsFreeCam(bool _isFreeCam);
	inline bool GetIsFreeCam() const { return freeCamInfo.isFreeCam; }

	/// @return Used to modify cameras ProjectionInfo
	ProjectionInfo& GetProjectionInfo() { return projectionInfo; }

private:
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 offset;
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector2 pitchConstraints; // NOTE: x = min, y = max in radians

	Transform* target;

	FreeCamInfo freeCamInfo;
	ProjectionInfo projectionInfo;
};

