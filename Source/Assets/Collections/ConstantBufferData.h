#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl.h>

#include "../Config/DirectXConfig.h"

struct ConstantBufferData
{
public:
	ConstantBufferData() : constantBuffer(nullptr), constantBufferType(DirectXConfig::ConstantBufferType::None) {}
	ConstantBufferData(Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer, DirectXConfig::ConstantBufferType _constantBufferType)
	{
		constantBuffer = std::move(_constantBuffer);
		constantBufferType = _constantBufferType;
	}
	~ConstantBufferData() = default;

	inline ID3D11Buffer* GetConstantBuffer() const { return constantBuffer.Get(); }
	inline DirectXConfig::ConstantBufferType GetConstantBufferType() const { return constantBufferType; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	DirectXConfig::ConstantBufferType constantBufferType;
};