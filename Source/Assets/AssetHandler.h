#pragma once

#include <d3d11.h>
#include <memory>
#include <SpriteFont.h>
#include <string>
#include <unordered_map>
#include <wrl.h>

#include "Material/Material.h"
#include "Model/Model.h"
#include "Config/DirectXConfig.h"

struct VertexShaderData
{
public:
	VertexShaderData() : vertexShader(nullptr), inputLayout(nullptr) {}
	VertexShaderData(Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout) 
	{
		vertexShader = std::move(_vertexShader);
		inputLayout = std::move(_inputLayout);
	}
	~VertexShaderData() = default;

	inline ID3D11VertexShader* GetVertexShader() const { return vertexShader.Get(); }
	inline ID3D11InputLayout* GetInputLayout() const { return inputLayout.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class AssetHandler
{
public:
	AssetHandler() = delete;
	~AssetHandler() = delete;
	AssetHandler(const AssetHandler&) = delete;
	AssetHandler& operator=(const AssetHandler&) = delete;

	/// @brief Loads all assets into memory so they can be used anywhere in the application
	static HRESULT LoadAssets();

	// TODO: Loader functions go here

	static inline const VertexShaderData& GetVertexShaderData(const std::string& name) { return vertexShaderLib[name]; }
	static inline ID3D11PixelShader* GetPixelShader(const std::string& name) { return pixelShaderLib[name].Get(); }
	static inline ID3D11ShaderResourceView* GetTexture(const std::string& name) { return textureLib[name].Get(); }
	static inline DirectX::SpriteFont* GetFont(const std::string& name) { return fontLib[name].get(); }

	static inline ID3D11SamplerState* GetSamplerState() { return samplerState.Get(); }

	static inline ID3D11Buffer* GetConstantBuffer(DirectXConfig::ConstantBufferType type) { return constantBufferLib[type].Get(); }
	static inline ID3D11DepthStencilState* GetDepthWrite(DirectXConfig::DepthWriteType type) { return depthWriteLib[type].Get(); }
	static inline ID3D11RasterizerState* GetCullingMode(DirectXConfig::CullingModeType type) { return cullingModeLib[type].Get(); }
	static inline ID3D11BlendState* GetBlendState(DirectXConfig::BlendStateType type) { return blendStateLib[type].Get(); }

	static inline Material* GetMaterial(const std::string& name) { return materialLib[name].get(); }
	static inline Model* GetModel(const std::string& name) { return modelLib[name].get(); }

private:
	static std::unordered_map<std::string, VertexShaderData> vertexShaderLib;
	static std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelShaderLib;
	static std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureLib;
	static std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> fontLib;

	static Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	static std::unordered_map<DirectXConfig::ConstantBufferType, Microsoft::WRL::ComPtr<ID3D11Buffer>> constantBufferLib;
	static std::unordered_map<DirectXConfig::DepthWriteType, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> depthWriteLib;
	static std::unordered_map<DirectXConfig::CullingModeType, Microsoft::WRL::ComPtr<ID3D11RasterizerState>> cullingModeLib;
	static std::unordered_map<DirectXConfig::BlendStateType, Microsoft::WRL::ComPtr<ID3D11BlendState>> blendStateLib;

	static std::unordered_map<std::string, std::unique_ptr<Material>> materialLib;
	static std::unordered_map<std::string, std::unique_ptr<Model>> modelLib;
};

