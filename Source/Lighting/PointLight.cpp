#include "PointLight.h"

PointLight::PointLight() : position(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)), strength(1.0f), isEnabled(false), padding{ 0.0f, 0.0f }
{
}

PointLight::PointLight(DirectX::XMVECTOR _colour, DirectX::XMVECTOR _position, float _strength) : Light(_colour), position(_position), 
																								  strength(_strength), isEnabled(true), padding{ 0.0f, 0.0f }
{
}

PointLight::~PointLight()
{
}
