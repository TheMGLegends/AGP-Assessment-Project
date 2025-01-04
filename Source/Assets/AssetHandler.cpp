#include "AssetHandler.h"

#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>

#include "ConstantBuffers/ConstantBuffers.h"
#include "Material/Material.h"
#include "Model/Model.h"
#include "ReadData.h"
#include "../Utilities/Debugging/DebugUtils.h"

using namespace ConstantBuffers;
using namespace DebugUtils;
using namespace DirectXConfig;
using namespace Microsoft::WRL;

std::unordered_map<std::string, VertexShaderData> AssetHandler::vertexShaderLib;
std::unordered_map<std::string, ComPtr<ID3D11PixelShader>> AssetHandler::pixelShaderLib;
std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> AssetHandler::textureLib;
std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> AssetHandler::fontLib;

ComPtr<ID3D11SamplerState> AssetHandler::samplerState;

std::unordered_map<ConstantBufferType, ConstantBufferData> AssetHandler::constantBufferLib;
std::unordered_map<DepthWriteType, ComPtr<ID3D11DepthStencilState>> AssetHandler::depthWriteLib;
std::unordered_map<CullingModeType, ComPtr<ID3D11RasterizerState>> AssetHandler::cullingModeLib;
std::unordered_map<BlendStateType, ComPtr<ID3D11BlendState>> AssetHandler::blendStateLib;

std::unordered_map<std::string, std::unique_ptr<Material>> AssetHandler::materialLib;
std::unordered_map<std::string, std::unique_ptr<Model>> AssetHandler::modelLib;

ID3D11Device* AssetHandler::deviceRef = nullptr;
ID3D11DeviceContext* AssetHandler::deviceContextRef = nullptr;

HRESULT AssetHandler::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	deviceRef = device;

	if (!deviceRef)
	{
		LogError("AssetHandler::Initialise(): Device is nullptr!");
		return E_FAIL;
	}

	deviceContextRef = deviceContext;

	if (!deviceContextRef)
	{
		LogError("AssetHandler::Initialise(): DeviceContext is nullptr!");
		return E_FAIL;
	}

	// INFO: Create the sampler state description
	D3D11_SAMPLER_DESC samplerDescription = {};
	ZeroMemory(&samplerDescription, sizeof(D3D11_SAMPLER_DESC));
	samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescription.MipLODBias = 0.0f;
	samplerDescription.MaxAnisotropy = 1;
	samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;

	samplerDescription.BorderColor[0] = 0;
	samplerDescription.BorderColor[1] = 0;
	samplerDescription.BorderColor[2] = 0;
	samplerDescription.BorderColor[3] = 0;

	samplerDescription.MinLOD = 0;
	samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	// INFO: Create the sampler state
	if (FAILED(deviceRef->CreateSamplerState(&samplerDescription, &samplerState)))
	{
		LogError("AssetHandler::Initialise(): Failed to create sampler state!");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT AssetHandler::LoadAssets()
{
	// INFO: Load Vertex Shaders
	if (FAILED(LoadVertexShader("Lit", L"Compiled Shaders/LitVertexShader.cso"))) return E_FAIL;
	if (FAILED(LoadVertexShader("Unlit", L"Compiled Shaders/UnlitVertexShader.cso"))) return E_FAIL;
	if (FAILED(LoadVertexShader("Reflective", L"Compiled Shaders/ReflectiveVertexShader.cso"))) return E_FAIL;
	if (FAILED(LoadVertexShader("Particle", L"Compiled Shaders/ParticleVertexShader.cso"))) return E_FAIL;
	if (FAILED(LoadVertexShader("Skybox", L"Compiled Shaders/SkyboxVertexShader.cso"))) return E_FAIL;

	// INFO: Load Pixel Shaders
	if (FAILED(LoadPixelShader("Lit", L"Compiled Shaders/LitPixelShader.cso"))) return E_FAIL;
	if (FAILED(LoadPixelShader("Unlit", L"Compiled Shaders/UnlitPixelShader.cso"))) return E_FAIL;
	if (FAILED(LoadPixelShader("Reflective", L"Compiled Shaders/ReflectivePixelShader.cso"))) return E_FAIL;
	if (FAILED(LoadPixelShader("Particle", L"Compiled Shaders/ParticlePixelShader.cso"))) return E_FAIL;
	if (FAILED(LoadPixelShader("Skybox", L"Compiled Shaders/SkyboxPixelShader.cso"))) return E_FAIL;

	// INFO: Load Textures
	if (FAILED(LoadTexture("Box", L"Resources/Textures/Box.bmp"))) return E_FAIL;
	if (FAILED(LoadTexture("DebugSkybox", L"Resources/Textures/DebugSkybox.dds", true))) return E_FAIL;
	if (FAILED(LoadTexture("GalaxySkybox", L"Resources/Textures/GalaxySkybox.dds", true))) return E_FAIL;
	
	// INFO: Load Fonts
	if (FAILED(LoadFont("SimpleFont", L"Resources/Fonts/SimpleFont.spriteFont"))) return E_FAIL;

	// INFO: Load Constant Buffers
	if (FAILED(LoadConstantBuffer(ConstantBufferType::LitVS))) return E_FAIL;
	if (FAILED(LoadConstantBuffer(ConstantBufferType::UnlitVS))) return E_FAIL;
	if (FAILED(LoadConstantBuffer(ConstantBufferType::ReflectiveVS))) return E_FAIL;
	if (FAILED(LoadConstantBuffer(ConstantBufferType::ReflectivePS))) return E_FAIL;
	if (FAILED(LoadConstantBuffer(ConstantBufferType::ParticleVS))) return E_FAIL;

	// INFO: Load Depth Writes
	if (FAILED(LoadDepthWrite(DepthWriteType::Enabled))) return E_FAIL;
	if (FAILED(LoadDepthWrite(DepthWriteType::Disabled))) return E_FAIL;

	// INFO: Load Culling Modes
	if (FAILED(LoadCullingMode(CullingModeType::NoneSolid))) return E_FAIL;
	if (FAILED(LoadCullingMode(CullingModeType::NoneWireframe))) return E_FAIL;
	if (FAILED(LoadCullingMode(CullingModeType::FrontSolid))) return E_FAIL;
	if (FAILED(LoadCullingMode(CullingModeType::FrontWireframe))) return E_FAIL;
	if (FAILED(LoadCullingMode(CullingModeType::BackSolid))) return E_FAIL;
	if (FAILED(LoadCullingMode(CullingModeType::BackWireframe))) return E_FAIL;

	// INFO: Load Blend States
	if (FAILED(LoadBlendState(BlendStateType::Enabled))) return E_FAIL;
	if (FAILED(LoadBlendState(BlendStateType::Disabled))) return E_FAIL;

	// INFO: Load Materials
	Material* debugSkyboxMaterial = new Material("Skybox", "Skybox", ConstantBufferType::UnlitVS, ConstantBufferInfo::ShaderType::Vertex, DepthWriteType::Disabled, CullingModeType::FrontSolid, BlendStateType::None, "DebugSkybox");
	if (FAILED(LoadMaterial("DebugSkyboxMaterial", debugSkyboxMaterial))) return E_FAIL;

	Material* galaxySkyboxMaterial = new Material("Skybox", "Skybox", ConstantBufferType::UnlitVS, ConstantBufferInfo::ShaderType::Vertex, DepthWriteType::Disabled, CullingModeType::FrontSolid, BlendStateType::None, "GalaxySkybox");
	if (FAILED(LoadMaterial("GalaxySkyboxMaterial", galaxySkyboxMaterial))) return E_FAIL;

	// TODO: TEST MATERIAL
	Material* testMaterial = new Material("Unlit", "Unlit", ConstantBufferType::UnlitVS, ConstantBufferInfo::ShaderType::Vertex, DepthWriteType::Enabled, CullingModeType::BackSolid, BlendStateType::None, "Box", "DebugSkybox");
	testMaterial->AddConstantBuffer(ConstantBufferType::ReflectivePS, ConstantBufferInfo::ShaderType::Pixel);
	if (FAILED(LoadMaterial("TestMaterial", testMaterial))) return E_FAIL;

	// INFO: Load Models
	if (FAILED(LoadModel("Cube", (char*)"Resources/Models/Cube.obj"))) return E_FAIL;
	if (FAILED(LoadModel("Sphere", (char*)"Resources/Models/Sphere.obj"))) return E_FAIL;

	return S_OK;
}

HRESULT AssetHandler::LoadVertexShader(const std::string& name, LPCWSTR filename)
{
	HRESULT hResult = { S_OK };

	// INFO: Load the compiled vertex shader
	auto vertexShaderBytecode = DX::ReadData(filename);

	// INFO: Create the vertex shader
	ComPtr<ID3D11VertexShader> vertexShader;
	hResult = deviceRef->CreateVertexShader(vertexShaderBytecode.data(), vertexShaderBytecode.size(), nullptr, &vertexShader);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadVertexShader(): Failed to create vertex shader! Filename: " + WStringToString(filename));
		return hResult;
	}

	// INFO: Create the vertex shader reflection
	ComPtr<ID3D11ShaderReflection> vertexShaderReflection;
	hResult = D3DReflect(vertexShaderBytecode.data(), vertexShaderBytecode.size(), IID_ID3D11ShaderReflection, &vertexShaderReflection);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadVertexShader(): Failed to create vertex shader reflection!");
		return hResult;
	}

	// INFO: Get the vertex shader description
	D3D11_SHADER_DESC vertexShaderDescription = {};
	hResult = vertexShaderReflection->GetDesc(&vertexShaderDescription);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadVertexShader(): Failed to get vertex shader description!");
		return hResult;
	}

	// INFO: Get the signature parameter descriptions
	D3D11_SIGNATURE_PARAMETER_DESC* signatureParameterDescriptions = new D3D11_SIGNATURE_PARAMETER_DESC[vertexShaderDescription.InputParameters]{ 0 };

	for (UINT i = 0; i < vertexShaderDescription.InputParameters; ++i)
	{
		vertexShaderReflection->GetInputParameterDesc(i, &signatureParameterDescriptions[i]);
	}

	// INFO: Create the input layout description
	D3D11_INPUT_ELEMENT_DESC* inputLayoutDescription = new D3D11_INPUT_ELEMENT_DESC[vertexShaderDescription.InputParameters]{ 0 };

	for (size_t i = 0; i < vertexShaderDescription.InputParameters; ++i)
	{
		inputLayoutDescription[i].SemanticName = signatureParameterDescriptions[i].SemanticName;
		inputLayoutDescription[i].SemanticIndex = signatureParameterDescriptions[i].SemanticIndex;

		if (signatureParameterDescriptions[i].ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
		{
			switch (signatureParameterDescriptions[i].Mask)
			{
			case 1: inputLayoutDescription[i].Format = DXGI_FORMAT_R32_FLOAT; break;
			case 3: inputLayoutDescription[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
			case 7: inputLayoutDescription[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case 15: inputLayoutDescription[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			default: break;
			}
		}

		inputLayoutDescription[i].InputSlot = 0;
		inputLayoutDescription[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputLayoutDescription[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDescription[i].InstanceDataStepRate = 0;
	}

	// INFO: Create the input layout
	ComPtr<ID3D11InputLayout> inputLayout;
	hResult = deviceRef->CreateInputLayout(inputLayoutDescription, vertexShaderDescription.InputParameters, vertexShaderBytecode.data(), vertexShaderBytecode.size(), &inputLayout);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadVertexShader(): Failed to create input layout!");
		return hResult;
	}

	// INFO: Create a new vertex shader data object
	VertexShaderData vertexShaderData(vertexShader, inputLayout);

	// INFO: Add the vertex shader data object to the library
	vertexShaderLib[name] = vertexShaderData;

	delete[] signatureParameterDescriptions;
	delete[] inputLayoutDescription;

	return S_OK;
}

HRESULT AssetHandler::LoadPixelShader(const std::string& name, LPCWSTR filename)
{
	HRESULT hResult = { S_OK };

	// INFO: Load the compiled pixel shader
	auto pixelShaderBytecode = DX::ReadData(filename);

	// INFO: Create the pixel shader
	ComPtr<ID3D11PixelShader> pixelShader;
	hResult = deviceRef->CreatePixelShader(pixelShaderBytecode.data(), pixelShaderBytecode.size(), nullptr, &pixelShader);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadPixelShader(): Failed to create pixel shader! Filename: " + WStringToString(filename));
		return hResult;
	}

	pixelShaderLib[name] = std::move(pixelShader);

	return S_OK;
}

HRESULT AssetHandler::LoadTexture(const std::string& name, LPCWSTR filename, bool isSkybox)
{
	HRESULT hResult = { S_OK };
	
	// INFO: Create the texture
	ComPtr<ID3D11ShaderResourceView> texture;

	// INFO: Load the texture using different methods depending on whether it is a skybox or not
	if (!isSkybox)
	{
		// INFO: Load the WIC texture
		hResult = DirectX::CreateWICTextureFromFile(deviceRef, deviceContextRef, filename, nullptr, &texture);
	}
	else
	{
		// INFO: Load the DDS texture
		hResult = DirectX::CreateDDSTextureFromFile(deviceRef, deviceContextRef, filename, nullptr, &texture);
	}

	if (FAILED(hResult))
	{
		std::string fileType = isSkybox ? "DDS" : "WIC";
		LogError("AssetHandler::LoadTexture(): Failed to create " + fileType + " texture! Filename: " + WStringToString(filename));
		return hResult;
	}

	textureLib[name] = std::move(texture);

	return S_OK;
}

HRESULT AssetHandler::LoadFont(const std::string& name, LPCWSTR filename)
{
	std::unique_ptr<DirectX::SpriteFont> font = std::make_unique<DirectX::SpriteFont>(deviceRef, filename);

	if (!font)
	{
		LogError("AssetHandler::LoadFont(): Failed to create sprite font! Filename: " + WStringToString(filename));
		return E_FAIL;
	}

	fontLib[name] = std::move(font);

	return S_OK;
}

HRESULT AssetHandler::LoadConstantBuffer(ConstantBufferType type)
{
	HRESULT hResult = { S_OK };

	// INFO: Create the constant buffer description
	D3D11_BUFFER_DESC constantBufferDescription = { 0 };

	// INFO: Different ByteWidth based on the constant buffer type
	switch (type)
	{
	case ConstantBufferType::LitVS:
		constantBufferDescription.ByteWidth = sizeof(LitVSBuffer);
		break;
	case ConstantBufferType::UnlitVS:
		constantBufferDescription.ByteWidth = sizeof(UnlitVSBuffer);
		break;
	case ConstantBufferType::ReflectiveVS:
		constantBufferDescription.ByteWidth = sizeof(ReflectiveVSBuffer);
		break;
	case ConstantBufferType::ReflectivePS:
		constantBufferDescription.ByteWidth = sizeof(ReflectivePSBuffer);
		break;
	case ConstantBufferType::ParticleVS:
		constantBufferDescription.ByteWidth = sizeof(ParticleVSBuffer);
		break;
	case ConstantBufferType::None:
	default:
		break;
	}

	constantBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDescription.CPUAccessFlags = 0;
	constantBufferDescription.MiscFlags = 0;
	constantBufferDescription.StructureByteStride = 0;

	// INFO: Create the constant buffer
	ComPtr<ID3D11Buffer> constantBuffer;
	hResult = deviceRef->CreateBuffer(&constantBufferDescription, nullptr, &constantBuffer);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadConstantBuffer(): Failed to create constant buffer!");
		return hResult;
	}

	ConstantBufferData constantBufferData(constantBuffer, type);

	constantBufferLib[type] = constantBufferData;

	return S_OK;
}

HRESULT AssetHandler::LoadDepthWrite(DepthWriteType type)
{
	HRESULT hResult = { S_OK };

	// INFO: Create the depth stencil state description
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription = { 0 };
	depthStencilDescription.DepthEnable = TRUE;

	// INFO: Different DepthWriteMask based on the depth write type
	switch (type)
	{
	case DepthWriteType::Enabled:
		depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	case DepthWriteType::Disabled:
		depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		break;
	case DepthWriteType::None:
	default:
		break;
	}

	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescription.StencilEnable = FALSE;
	depthStencilDescription.StencilReadMask = 0;
	depthStencilDescription.StencilWriteMask = 0;

	depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// INFO: Create the depth stencil state
	ComPtr<ID3D11DepthStencilState> depthStencilState;
	hResult = deviceRef->CreateDepthStencilState(&depthStencilDescription, &depthStencilState);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadDepthWrite(): Failed to create depth stencil state!");
		return hResult;
	}

	depthWriteLib[type] = std::move(depthStencilState);

	return S_OK;
}

HRESULT AssetHandler::LoadCullingMode(CullingModeType type)
{
	HRESULT hResult = { S_OK };

	// INFO: Create the rasterizer state description
	D3D11_RASTERIZER_DESC rasterizerDescription = {};
	ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

	// INFO: Different FillMode and CullMode based on the culling mode type
	switch (type)
	{
	case CullingModeType::NoneSolid:
		rasterizerDescription.FillMode = D3D11_FILL_SOLID;
		rasterizerDescription.CullMode = D3D11_CULL_NONE;
		break;
	case CullingModeType::NoneWireframe:
		rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDescription.CullMode = D3D11_CULL_NONE;
		break;
	case CullingModeType::FrontSolid:
		rasterizerDescription.FillMode = D3D11_FILL_SOLID;
		rasterizerDescription.CullMode = D3D11_CULL_FRONT;
		break;
	case CullingModeType::FrontWireframe:
		rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDescription.CullMode = D3D11_CULL_FRONT;
		break;
	case CullingModeType::BackSolid:
		rasterizerDescription.FillMode = D3D11_FILL_SOLID;
		rasterizerDescription.CullMode = D3D11_CULL_BACK;
		break;
	case CullingModeType::BackWireframe:
		rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDescription.CullMode = D3D11_CULL_BACK;
		break;
	default:
		break;
	}

	rasterizerDescription.FrontCounterClockwise = FALSE;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;
	rasterizerDescription.DepthClipEnable = TRUE;
	rasterizerDescription.ScissorEnable = FALSE;
	rasterizerDescription.MultisampleEnable = FALSE;
	rasterizerDescription.AntialiasedLineEnable = FALSE;

	// INFO: Create the rasterizer state
	ComPtr<ID3D11RasterizerState> rasterizerState;
	hResult = deviceRef->CreateRasterizerState(&rasterizerDescription, &rasterizerState);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadCullingMode(): Failed to create rasterizer state!");
		return hResult;
	}

	cullingModeLib[type] = std::move(rasterizerState);

	return S_OK;
}

HRESULT AssetHandler::LoadBlendState(BlendStateType type)
{
	HRESULT hResult = { S_OK };

	// INFO: Create the blend state description
	D3D11_BLEND_DESC blendDescription = { 0 };
	blendDescription.AlphaToCoverageEnable = FALSE;
	blendDescription.IndependentBlendEnable = FALSE;

	// INFO: Different RenderTarget[0] based on the blend state type
	switch (type)
	{
	case BlendStateType::Enabled:
		blendDescription.RenderTarget[0].BlendEnable = TRUE;
		break;
	case BlendStateType::Disabled:
		blendDescription.RenderTarget[0].BlendEnable = FALSE;
		break;
	case BlendStateType::None:
	default:
		break;
	}

	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// INFO: Create the blend state
	ComPtr<ID3D11BlendState> blendState;
	hResult = deviceRef->CreateBlendState(&blendDescription, &blendState);

	if (FAILED(hResult))
	{
		LogError("AssetHandler::LoadBlendState(): Failed to create blend state!");
		return hResult;
	}

	blendStateLib[type] = std::move(blendState);

	return S_OK;
}

HRESULT AssetHandler::LoadMaterial(const std::string& name, Material* _material)
{
	// INFO: Create a new material object
	std::unique_ptr<Material> material(_material);

	if (!material)
	{
		LogError("AssetHandler::LoadMaterial(): Failed to create material!");
		return E_FAIL;
	}

	materialLib[name] = std::move(material);

	return S_OK;
}

HRESULT AssetHandler::LoadModel(const std::string& name, char* filename)
{
	// INFO: Create a new model object
	std::unique_ptr<Model> model = std::make_unique<Model>(filename, deviceRef, deviceContextRef);

	if (!model)
	{
		LogError("AssetHandler::LoadModel(): Failed to create model! Filename: " + std::string(filename));
		return E_FAIL;
	}

	modelLib[name] = std::move(model);

	return S_OK;
}

const VertexShaderData& AssetHandler::GetVertexShaderData(const std::string& name)
{
	if (vertexShaderLib.find(name) != vertexShaderLib.end())
		return vertexShaderLib[name];

	LogWarning("AssetHandler::GetVertexShaderData(): Vertex shader not found! Name: " + name);
	return VertexShaderData::EMPTY;
}

ID3D11PixelShader* AssetHandler::GetPixelShader(const std::string& name)
{
	if (pixelShaderLib.find(name) != pixelShaderLib.end())
		return pixelShaderLib[name].Get();

	LogWarning("AssetHandler::GetPixelShader(): Pixel shader not found! Name: " + name);
	return nullptr;
}

ID3D11ShaderResourceView* AssetHandler::GetTexture(const std::string& name)
{
	if (textureLib.find(name) != textureLib.end())
		return textureLib[name].Get();

	LogWarning("AssetHandler::GetTexture(): Texture not found! Name: " + name);
	return nullptr;
}

DirectX::SpriteFont* AssetHandler::GetFont(const std::string& name)
{
	if (fontLib.find(name) != fontLib.end())
		return fontLib[name].get();

	LogWarning("AssetHandler::GetFont(): Font not found! Name: " + name);
	return nullptr;
}

const ConstantBufferData& AssetHandler::GetConstantBufferData(ConstantBufferType type)
{
	if (type == ConstantBufferType::None)
		return ConstantBufferData::EMPTY;

	if (constantBufferLib.find(type) != constantBufferLib.end())
		return constantBufferLib[type];

	LogWarning("AssetHandler::GetConstantBufferData(): Constant buffer data not found! Type: " + std::to_string(static_cast<int>(type)));
	return ConstantBufferData::EMPTY;
}

ID3D11DepthStencilState* AssetHandler::GetDepthWrite(DepthWriteType type)
{
	if (type == DepthWriteType::None)
		return nullptr;

	if (depthWriteLib.find(type) != depthWriteLib.end())
		return depthWriteLib[type].Get();

	LogWarning("AssetHandler::GetDepthWrite(): Depth write not found! Type: " + std::to_string(static_cast<int>(type)));
	return nullptr;
}

ID3D11RasterizerState* AssetHandler::GetCullingMode(CullingModeType type)
{
	if (cullingModeLib.find(type) != cullingModeLib.end())
		return cullingModeLib[type].Get();

	LogWarning("AssetHandler::GetCullingMode(): Culling mode not found! Type: " + std::to_string(static_cast<int>(type)));
	return nullptr;
}

ID3D11BlendState* AssetHandler::GetBlendState(BlendStateType type)
{
	if (type == BlendStateType::None)
		return nullptr;

	if (blendStateLib.find(type) != blendStateLib.end())
		return blendStateLib[type].Get();

	LogWarning("AssetHandler::GetBlendState(): Blend state not found! Type: " + std::to_string(static_cast<int>(type)));
	return nullptr;
}

Material* AssetHandler::GetMaterial(const std::string& name)
{
	if (materialLib.find(name) != materialLib.end())
		return materialLib[name].get();

	LogWarning("AssetHandler::GetMaterial(): Material not found! Name: " + name);
	return nullptr;
}

Model* AssetHandler::GetModel(const std::string& name)
{
	if (modelLib.find(name) != modelLib.end())
		return modelLib[name].get();

	LogWarning("AssetHandler::GetModel(): Model not found! Name: " + name);
	return nullptr;
}
