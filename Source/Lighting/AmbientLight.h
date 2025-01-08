#pragma once

#include "Core/Light.h"

class AmbientLight : public Light
{
public:
	using Light::Light;
	~AmbientLight();
};

