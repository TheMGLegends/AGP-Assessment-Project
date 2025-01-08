#pragma once

#include <DirectXMath.h>

class Light
{
public:
	Light();
	Light(DirectX::XMVECTOR _colour);
	~Light();

	inline void SetColour(const DirectX::XMVECTOR& _colour) { colour = _colour; }
	inline const DirectX::XMVECTOR& GetColour() const { return colour; }

protected:
	DirectX::XMVECTOR colour;
};

