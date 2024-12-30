#pragma once

#include <d3d11.h>
#include <string>

#include "../Config/DirectXConfig.h"

class Material
{
public:
	Material(std::string vertexShaderName, std::string pixelShaderName, DirectXConfig::ConstantBufferType constantBufferType, 
		     DirectXConfig::DepthWriteType depthWriteType, DirectXConfig::CullingModeType cullingModeType, 
		     DirectXConfig::BlendStateType blendStateType, std::string textureName, std::string skyboxTextureName);
	~Material();

	inline ID3D11Buffer* GetConstantBuffer() const { return constantBuffer; }
	inline DirectXConfig::ConstantBufferType GetConstantBufferType() const { return constantBufferType; }

	/// @brief Set all material properties ready for rendering
	void Set(ID3D11DeviceContext* deviceContext);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;

	ID3D11Buffer* constantBuffer;
	DirectXConfig::ConstantBufferType constantBufferType;

	ID3D11DepthStencilState* depthWrite;
	ID3D11RasterizerState* cullingMode;
	ID3D11BlendState* blendState;

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* skyboxTexture;
	ID3D11SamplerState* sampler;
};

