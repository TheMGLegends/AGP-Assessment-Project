#include "Material.h"

#include "../AssetHandler.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace DebugUtils;
using namespace DirectXConfig;

Material::Material(const std::string& vertexShaderName, const std::string& pixelShaderName, ConstantBufferType constantBufferType, 
				   ConstantBufferInfo::ShaderType shaderType, DepthWriteType depthWriteType, CullingModeType cullingModeType, 
				   BlendStateType blendStateType, const std::string& textureName, const std::string& skyboxTextureName) : vertexShader(nullptr),
																														  pixelShader(nullptr),
																														  inputLayout(nullptr),
																														  depthWrite(nullptr),
																														  cullingMode(nullptr),
																														  blendState(nullptr),
																														  texture(nullptr),
																														  reflectedTexture(nullptr),
																														  sampler(nullptr),
																														  reflectionAmount(0.5f)
{
	// INFO: Use asset handler to populate the material properties from the asset files

	const VertexShaderData& vertexShaderData = AssetHandler::GetVertexShaderData(vertexShaderName);
	vertexShader = vertexShaderData.GetVertexShader();
	inputLayout = vertexShaderData.GetInputLayout();

	pixelShader = AssetHandler::GetPixelShader(pixelShaderName);

	AddConstantBuffer(constantBufferType, shaderType);

	depthWrite = AssetHandler::GetDepthWrite(depthWriteType);
	cullingMode = AssetHandler::GetCullingMode(cullingModeType);
	blendState = AssetHandler::GetBlendState(blendStateType);

	texture = AssetHandler::GetTexture(textureName);

	// INFO: Only use the reflected texture if the material is reflective
	if (HasConstantBuffer(ConstantBufferType::ReflectiveVS))
		reflectedTexture = AssetHandler::GetTexture(skyboxTextureName);

	sampler = AssetHandler::GetSamplerState();
}

Material::~Material()
{
}

ID3D11Buffer* Material::GetConstantBuffer(DirectXConfig::ConstantBufferType type)
{
	if (HasConstantBuffer(type))
		return constantBuffers[type].buffer;

	return nullptr;
}

void Material::AddConstantBuffer(DirectXConfig::ConstantBufferType constantBufferType, ConstantBufferInfo::ShaderType shaderType)
{
	const ConstantBufferData& constantBufferData = AssetHandler::GetConstantBufferData(constantBufferType);
	ConstantBufferInfo constantBufferInfo = { constantBufferData.GetConstantBuffer(), shaderType };
	constantBuffers[constantBufferData.GetConstantBufferType()] = constantBufferInfo;
}

void Material::SetTexture(const std::string& textureName)
{
	texture = AssetHandler::GetTexture(textureName);
}

void Material::SetReflectedTexture(const std::string& reflectedTextureName)
{
	reflectedTexture = AssetHandler::GetTexture(reflectedTextureName);
}

void Material::Set(ID3D11DeviceContext* deviceContext)
{
	if (!deviceContext)
	{
		LogError("Material::Set(): DeviceContext is nullptr!");
		return;
	}

	if (inputLayout)
		deviceContext->IASetInputLayout(inputLayout);

	if (vertexShader)
		deviceContext->VSSetShader(vertexShader, nullptr, 0);

	if (pixelShader)
		deviceContext->PSSetShader(pixelShader, nullptr, 0);

	// INFO: Set the constant buffer for each constant buffer found in the material
	for (auto& cb : constantBuffers)
	{
		ConstantBufferInfo constantBufferInfo = cb.second;

		if (constantBufferInfo.shaderType == ConstantBufferInfo::ShaderType::Vertex)
			deviceContext->VSSetConstantBuffers(0, 1, &constantBufferInfo.buffer);
		else if (constantBufferInfo.shaderType == ConstantBufferInfo::ShaderType::Pixel)
			deviceContext->PSSetConstantBuffers(0, 1, &constantBufferInfo.buffer);
	}

	if (texture)
		deviceContext->PSSetShaderResources(0, 1, &texture);

	if (reflectedTexture && HasConstantBuffer(ConstantBufferType::ReflectiveVS))
		deviceContext->PSSetShaderResources(1, 1, &reflectedTexture);

	if (sampler)
		deviceContext->PSSetSamplers(0, 1, &sampler);

	if (cullingMode)
		deviceContext->RSSetState(cullingMode);

	if (depthWrite)
		deviceContext->OMSetDepthStencilState(depthWrite, 0);

	if (blendState)
		deviceContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
}
