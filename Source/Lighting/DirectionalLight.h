#pragma once

#include "Core/Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(DirectX::XMVECTOR _colour, DirectX::XMVECTOR _direction);
	virtual ~DirectionalLight() override;

	inline void SetDirection(const DirectX::XMVECTOR& _direction) { direction = _direction; }
	inline const DirectX::XMVECTOR& GetDirection() const { return direction; }

private:
	DirectX::XMVECTOR direction;
};

