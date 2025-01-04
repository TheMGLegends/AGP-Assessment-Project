#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl.h>

#include "../Config/DirectXConfig.h"

struct ConstantBufferData
{
public:
	ConstantBufferData();
	ConstantBufferData(Microsoft::WRL::ComPtr<ID3D11Buffer> _constantBuffer, DirectXConfig::ConstantBufferType _constantBufferType);
	~ConstantBufferData();

	inline ID3D11Buffer* GetConstantBuffer() const { return constantBuffer.Get(); }
	inline DirectXConfig::ConstantBufferType GetConstantBufferType() const { return constantBufferType; }

public:
	static const ConstantBufferData EMPTY;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	DirectXConfig::ConstantBufferType constantBufferType;
};