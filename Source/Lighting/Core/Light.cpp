#include "Light.h"

using namespace DirectX;

Light::Light() : colour(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f))
{
}

Light::Light(XMVECTOR _colour) : colour(_colour)
{
}

Light::~Light()
{
}
