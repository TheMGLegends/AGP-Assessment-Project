#include "Material.h"

//#include "../AssetHandler.h"

using namespace DirectXConfig;

Material::Material(std::string vertexShaderName, std::string pixelShaderName, 
				   ConstantBufferType constantBufferType, DepthWriteType depthWriteType, 
				   CullingModeType cullingModeType, BlendStateType blendStateType, 
	               std::string textureName, std::string skyboxTextureName) : vertexShader(nullptr),
																			 pixelShader(nullptr),
																			 inputLayout(nullptr),
																			 constantBuffer(nullptr),
																			 depthWrite(nullptr),
																			 cullingMode(nullptr),
																			 blendState(nullptr),
																			 texture(nullptr),
																			 skyboxTexture(nullptr),
																			 sampler(nullptr)
{
	// TODO: Use asset handler to populate the material properties from the asset files
}

void Material::Set(ID3D11DeviceContext* deviceContext)
{
	// TODO: Set all material properties on the device context ready for rendering
}
