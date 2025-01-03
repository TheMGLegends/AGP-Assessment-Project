#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : direction(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
{
}

DirectionalLight::DirectionalLight(DirectX::XMVECTOR _colour, DirectX::XMVECTOR _direction) : Light(_colour), direction(_direction)
{
}

DirectionalLight::~DirectionalLight()
{
}
