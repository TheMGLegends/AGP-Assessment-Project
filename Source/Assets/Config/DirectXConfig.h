#pragma once

namespace DirectXConfig
{
	enum class ConstantBufferType
	{
		None = 0,

		LitVS,
		UnlitVS,
		ReflectiveVS,
		ReflectivePS,
		ParticleVS
	};

	enum class DepthWriteType
	{
		None = 0,

		Enabled,
		Disabled
	};

	enum class CullingModeType
	{
		NoneSolid,
		NoneWireframe,

		FrontSolid,
		FrontWireframe,

		BackSolid,
		BackWireframe
	};

	enum class BlendStateType
	{
		None = 0,

		Enabled,
		Disabled
	};
}