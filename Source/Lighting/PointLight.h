#pragma once

#include "Core/Light.h"

#include <Windows.h>

constexpr int MAX_POINT_LIGHTS = 8;

class PointLight : public Light
{
public:
	/// @brief Default construction has point light disabled
	PointLight();
	PointLight(DirectX::XMVECTOR _colour, DirectX::XMVECTOR _position, float _strength);
	virtual ~PointLight() override;

	inline void SetPosition(const DirectX::XMVECTOR& _position) { position = _position; }
	inline const DirectX::XMVECTOR& GetPosition() const { return position; }

	inline void SetStrength(float _strength) { strength = _strength; }
	inline float GetStrength() const { return strength; }

	inline void SetIsEnabled(BOOL _isEnabled) { isEnabled = _isEnabled; }
	inline BOOL GetIsEnabled() const { return isEnabled; }

private:
	DirectX::XMVECTOR position;
	float strength;
	BOOL isEnabled;

	float padding[2];
};

