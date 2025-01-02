#pragma once

#include <d3d11.h>
#include <string>
#include <unordered_map>

#include "../Config/DirectXConfig.h"

struct ConstantBufferInfo
{
public:
	enum class ShaderType
	{
		Vertex,
		Pixel
	};
		
public:
	ConstantBufferInfo() : buffer(nullptr), shaderType(ShaderType::Vertex) {}
	ConstantBufferInfo(ID3D11Buffer* _buffer, ShaderType _shaderType) : buffer(_buffer), shaderType(_shaderType) {}

public:
	ID3D11Buffer* buffer;
	ShaderType shaderType;
};

class Material
{
public:
	/// @param constantBufferType The initial type of constant buffer to use (More can be added after creation)
	Material(std::string vertexShaderName, std::string pixelShaderName, DirectXConfig::ConstantBufferType constantBufferType, 
			 ConstantBufferInfo::ShaderType shaderType, DirectXConfig::DepthWriteType depthWriteType, DirectXConfig::CullingModeType cullingModeType, 
		     DirectXConfig::BlendStateType blendStateType, std::string textureName, std::string skyboxTextureName);
	~Material();

	inline std::unordered_map<DirectXConfig::ConstantBufferType, ConstantBufferInfo>& GetConstantBuffers() { return constantBuffers; }
	ID3D11Buffer* GetConstantBuffer(DirectXConfig::ConstantBufferType type);

	inline bool HasConstantBuffer(DirectXConfig::ConstantBufferType type) { return constantBuffers.find(type) != constantBuffers.end(); }

	/// @brief Use to add additional constant buffers to the material
	void AddConstantBuffer(DirectXConfig::ConstantBufferType constantBufferType, ConstantBufferInfo::ShaderType shaderType);

	inline void SetReflectionAmount(float amount) { reflectionAmount = amount; }
	inline float GetReflectionAmount() const { return reflectionAmount; }

	/// @brief Set all material properties ready for rendering
	void Set(ID3D11DeviceContext* deviceContext);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;

	std::unordered_map<DirectXConfig::ConstantBufferType, ConstantBufferInfo> constantBuffers;

	ID3D11DepthStencilState* depthWrite;
	ID3D11RasterizerState* cullingMode;
	ID3D11BlendState* blendState;

	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* skyboxTexture;
	ID3D11SamplerState* sampler;

	float reflectionAmount; // NOTE: Used by the reflection shaders
};

