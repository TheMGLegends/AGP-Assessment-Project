#pragma once

#include <string>
#include <SpriteFont.h>
#include <unordered_map>

#include "Collections/ConstantBufferData.h"
#include "Collections/VertexShaderData.h"

class Material;
class Model;
class SpriteFont;

class AssetHandler
{
public:
	AssetHandler() = delete;
	~AssetHandler() = delete;
	AssetHandler(const AssetHandler&) = delete;
	AssetHandler& operator=(const AssetHandler&) = delete;

	static HRESULT Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	/// @brief Loads all assets into memory so they can be used anywhere in the application
	static HRESULT LoadAssets();


	// INFO: Loaders

	static HRESULT LoadVertexShader(const std::string& name, LPCWSTR filename);
	static HRESULT LoadPixelShader(const std::string& name, LPCWSTR filename);
	static HRESULT LoadTexture(const std::string& name, LPCWSTR filename, bool isSkybox = false);
	static HRESULT LoadFont(const std::string& name, LPCWSTR filename);

	static HRESULT LoadConstantBuffer(DirectXConfig::ConstantBufferType type);
	static HRESULT LoadDepthWrite(DirectXConfig::DepthWriteType type);
	static HRESULT LoadCullingMode(DirectXConfig::CullingModeType type);
	static HRESULT LoadBlendState(DirectXConfig::BlendStateType type);

	static HRESULT LoadMaterial(const std::string& name, Material* _material);
	static HRESULT LoadModel(const std::string& name, char* filename);


	// INFO: Getters

	static const VertexShaderData& GetVertexShaderData(const std::string& name);
	static ID3D11PixelShader* GetPixelShader(const std::string& name);
	static ID3D11ShaderResourceView* GetTexture(const std::string& name);
	static DirectX::SpriteFont* GetFont(const std::string& name);

	static ID3D11SamplerState* GetSamplerState() { return samplerState.Get(); }

	static const ConstantBufferData& GetConstantBufferData(DirectXConfig::ConstantBufferType type);
	static ID3D11DepthStencilState* GetDepthWrite(DirectXConfig::DepthWriteType type);
	static ID3D11RasterizerState* GetCullingMode(DirectXConfig::CullingModeType type);
	static ID3D11BlendState* GetBlendState(DirectXConfig::BlendStateType type);

	static Material* GetMaterial(const std::string& name);
	static Model* GetModel(const std::string& name);

private:
	static std::unordered_map<std::string, VertexShaderData> vertexShaderLib;
	static std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelShaderLib;
	static std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureLib;
	static std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> fontLib;

	static Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	static std::unordered_map<DirectXConfig::ConstantBufferType, ConstantBufferData> constantBufferLib;
	static std::unordered_map<DirectXConfig::DepthWriteType, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> depthWriteLib;
	static std::unordered_map<DirectXConfig::CullingModeType, Microsoft::WRL::ComPtr<ID3D11RasterizerState>> cullingModeLib;
	static std::unordered_map<DirectXConfig::BlendStateType, Microsoft::WRL::ComPtr<ID3D11BlendState>> blendStateLib;

	static std::unordered_map<std::string, std::unique_ptr<Material>> materialLib;
	static std::unordered_map<std::string, std::unique_ptr<Model>> modelLib;

	static ID3D11Device* deviceRef;
	static ID3D11DeviceContext* deviceContextRef;
};

