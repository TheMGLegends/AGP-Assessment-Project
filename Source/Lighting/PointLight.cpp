#include "PointLight.h"

PointLight::PointLight() : position(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)), strength(1.0f), isEnabled(FALSE)
{
}

PointLight::PointLight(DirectX::XMVECTOR _colour, DirectX::XMVECTOR _position, float _strength) : Light(_colour), position(_position), 
																								  strength(_strength), isEnabled(TRUE)
{
}

PointLight::~PointLight()
{
}
