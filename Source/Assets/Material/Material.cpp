#include "Material.h"

#include "../AssetHandler.h"

using namespace DirectXConfig;

Material::Material(std::string vertexShaderName, std::string pixelShaderName, 
				   ConstantBufferType constantBufferType, DepthWriteType depthWriteType, 
				   CullingModeType cullingModeType, BlendStateType blendStateType, 
	               std::string textureName, std::string skyboxTextureName) : vertexShader(nullptr),
																			 pixelShader(nullptr),
																			 inputLayout(nullptr),
																			 constantBuffer(nullptr),
																			 constantBufferType(constantBufferType),
																			 depthWrite(nullptr),
																			 cullingMode(nullptr),
																			 blendState(nullptr),
																			 texture(nullptr),
																			 skyboxTexture(nullptr),
																			 sampler(nullptr)
{
	// INFO: Use asset handler to populate the material properties from the asset files

	const VertexShaderData& vertexShaderData = AssetHandler::GetVertexShaderData(vertexShaderName);
	vertexShader = vertexShaderData.GetVertexShader();
	inputLayout = vertexShaderData.GetInputLayout();

	pixelShader = AssetHandler::GetPixelShader(pixelShaderName);

	const ConstantBufferData& constantBufferData = AssetHandler::GetConstantBufferData(constantBufferType);
	constantBuffer = constantBufferData.GetConstantBuffer();
	constantBufferType = constantBufferData.GetConstantBufferType();

	depthWrite = AssetHandler::GetDepthWrite(depthWriteType);
	cullingMode = AssetHandler::GetCullingMode(cullingModeType);
	blendState = AssetHandler::GetBlendState(blendStateType);

	texture = AssetHandler::GetTexture(textureName);
	skyboxTexture = AssetHandler::GetTexture(skyboxTextureName);

	sampler = AssetHandler::GetSamplerState();
}

void Material::Set(ID3D11DeviceContext* deviceContext)
{
	// TODO: Set all material properties on the device context ready for rendering
}
