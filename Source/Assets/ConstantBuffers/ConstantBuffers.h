#pragma once

#include <DirectXMath.h>

//#include "../../Components/Lighting/Light.h"

namespace ConstantBuffers
{
	struct ParticleBuffer
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMFLOAT4 colour;
	};

	struct UnlitBuffer
	{
		DirectX::XMMATRIX wvp;
	};

	struct LitBuffer
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMVECTOR ambientLightColour;
		DirectX::XMVECTOR directionalLightColour;
		DirectX::XMVECTOR directionalLightDirection;
		// TODO: Point Lights
		// PointLight pointLights[LightConfig::MAX_LIGHTS];
	};

	struct ReflectiveBuffer
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX wv;
		DirectX::XMVECTOR ambientLightColour;
		DirectX::XMVECTOR directionalLightColour;
		DirectX::XMVECTOR directionalLightDirection;
		// TODO: Point Lights
		// PointLight pointLights[LightConfig::MAX_LIGHTS];
	};
}