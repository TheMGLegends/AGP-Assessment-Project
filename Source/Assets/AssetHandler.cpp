#include "AssetHandler.h"

using namespace DirectXConfig;
using namespace Microsoft::WRL;

std::unordered_map<std::string, VertexShaderData> AssetHandler::vertexShaderLib;
std::unordered_map<std::string, ComPtr<ID3D11PixelShader>> AssetHandler::pixelShaderLib;
std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> AssetHandler::textureLib;
std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> AssetHandler::fontLib;

ComPtr<ID3D11SamplerState> AssetHandler::samplerState;

std::unordered_map<ConstantBufferType, ComPtr<ID3D11Buffer>> AssetHandler::constantBufferLib;
std::unordered_map<DepthWriteType, ComPtr<ID3D11DepthStencilState>> AssetHandler::depthWriteLib;
std::unordered_map<CullingModeType, ComPtr<ID3D11RasterizerState>> AssetHandler::cullingModeLib;
std::unordered_map<BlendStateType, ComPtr<ID3D11BlendState>> AssetHandler::blendStateLib;

std::unordered_map<std::string, std::unique_ptr<Material>> AssetHandler::materialLib;
//std::unordered_map<std::string, std::unique_ptr<Model>> AssetHandler::modelLib;

HRESULT AssetHandler::LoadAssets()
{
	// TODO: Load all assets here

	return S_OK;
}
