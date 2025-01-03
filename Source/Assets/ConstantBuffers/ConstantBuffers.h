#pragma once

#include <DirectXMath.h>

#include "../../Lighting/PointLight.h"
#include "../../Utilities/MathUtils.h"

namespace ConstantBuffers
{
	struct ParticleVSBuffer
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMFLOAT4 colour;
	};

	struct UnlitVSBuffer
	{
		DirectX::XMMATRIX wvp;
	};

	struct LitVSBuffer
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMVECTOR ambientLightColour;
		DirectX::XMVECTOR directionalLightColour;
		DirectX::XMVECTOR directionalLightDirection;
		PointLight pointLights[MAX_POINT_LIGHTS];
	};

	struct ReflectiveVSBuffer
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX wv;
		DirectX::XMVECTOR ambientLightColour;
		DirectX::XMVECTOR directionalLightColour;
		DirectX::XMVECTOR directionalLightDirection;
		PointLight pointLights[MAX_POINT_LIGHTS];
	};

	struct ReflectivePSBuffer
	{
		float reflectionAmount;
		float padding[3];

		ReflectivePSBuffer& operator=(float _reflectionAmount)
		{
			this->reflectionAmount = MathUtils::Clamp(_reflectionAmount, 0.0f, 1.0f);
			return *this;
		}
	};
}