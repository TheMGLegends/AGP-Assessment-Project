#include "Renderer.h"

#include "Skybox.h"
#include "../../Assets/AssetHandler.h"
#include "../../Assets/ConstantBuffers/ConstantBuffers.h"
#include "../../Assets/Material/Material.h"
#include "../../Components/Core/ComponentHandler.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Game/Camera/Camera.h"
#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Game/GameObjects/Default/Particle.h"
#include "../../Lighting/PointLight.h"
#include "../../Scene/Core/Scene.h"
#include "../../UI/Core/UserInterfaceElement.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace ConstantBuffers;
using namespace DebugUtils;
using namespace DirectX;
using namespace DirectXConfig;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

Renderer::Renderer() : device(nullptr), deviceContext(nullptr), swapChain(nullptr), 
					   renderTargetView(nullptr), depthStencilView(nullptr), viewport({ 0 }), 
					   spriteBatch(nullptr)
{
}

Renderer::~Renderer()
{
}

HRESULT Renderer::Initialise(HWND hWnd)
{
	HRESULT hResult = { S_OK };

	// INFO: Create the swap chain description
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// INFO: Create the device, device context, swap chain and front/back buffers
	hResult = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
	);

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to create device and swapchain!");
		return hResult;
	}

	// INFO: Get the back buffer
	ComPtr<ID3D11Resource> backBuffer;

	hResult = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to get back buffer!");
		return hResult;
	}

	// INFO: Create the render target view
	hResult = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to create render target view!");
		return hResult;
	}

	// INFO: Get the window dimensions
	RECT rc = { 0 };
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// INFO: Create the depth stencil buffer description
	D3D11_TEXTURE2D_DESC dsb = { 0 };
	dsb.Width = width;
	dsb.Height = height;
	dsb.MipLevels = 1;
	dsb.ArraySize = 1;
	dsb.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	dsb.SampleDesc.Count = scd.SampleDesc.Count;
	dsb.SampleDesc.Quality = scd.SampleDesc.Quality;

	dsb.Usage = D3D11_USAGE_DEFAULT;
	dsb.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsb.CPUAccessFlags = 0;
	dsb.MiscFlags = 0;

	// INFO: Create the depth stencil buffer
	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	hResult = device->CreateTexture2D(&dsb, nullptr, &depthStencilBuffer);

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to create depth stencil buffer!");
		return hResult;
	}

	// INFO: Create the depth stencil view description
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Format = dsb.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;

	// INFO: Create the depth stencil view
	hResult = device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvd, &depthStencilView);

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to create depth stencil view!");
		return hResult;
	}

	// INFO: Set the render target view and depth stencil view
	deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	// INFO: Set the viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);

	// INFO: Create the sprite batch
	spriteBatch = std::make_unique<SpriteBatch>(deviceContext.Get());

	if (!spriteBatch)
	{
		LogError("Renderer::Initialise(): Failed to create sprite batch!");
		return E_FAIL;
	}

	// INFO: Initialise the AssetHandler
	hResult = AssetHandler::Initialise(device.Get(), deviceContext.Get());

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to initialise the AssetHandler!");
		return hResult;
	}

	// INFO: Load all assets via the AssetHandler
	hResult = AssetHandler::LoadAssets();

	if (FAILED(hResult))
	{
		LogError("Renderer::Initialise(): Failed to load assets!");
		return hResult;
	}

	return S_OK;
}

void Renderer::RenderFrame(Scene* scene)
{
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), Colors::DarkSeaGreen);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// INFO: Get all the properties we need
	Camera* camera = scene->GetCamera();
	Skybox* skybox = scene->GetSkybox();
	Vector3 cameraPosition = camera->GetPosition();

	if (!camera || !skybox)
	{
		LogWarning("Renderer::RenderFrame(): Camera/Skybox is nullptr!");
		return;
	}

	// INFO: Get all the matrices we need
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(camera->GetPosition());
	XMMATRIX viewMatrix = camera->GetViewMatrix();
	XMMATRIX projectionMatrix = camera->GetProjectionMatrix();

	// INFO: Get the lights we need
	const AmbientLight& ambientLight = scene->GetAmbientLight();
	const DirectionalLight& directionalLight = scene->GetDirectionalLight();
	const std::array<PointLight, MAX_POINT_LIGHTS>& pointLights = scene->GetPointLights();

	// INFO: Render the skybox
	skybox->Draw(deviceContext.Get(), translationMatrix, viewMatrix, projectionMatrix);

	// INFO: Render all mesh components
	for (auto& weakMesh : ComponentHandler::GetMeshes())
	{
		std::shared_ptr<Mesh> mesh = weakMesh.lock();
		Material* material = mesh->GetMaterial();

		GameObject* owningGameObject = mesh->GetGameObject();
		std::shared_ptr<Transform> owningGameObjectTransform = owningGameObject->transform.lock();

		if (!mesh || !material || !owningGameObject || !owningGameObjectTransform)
		{
			LogWarning("Renderer::RenderFrame(): Mesh/Material/OwningGameObject/OwningGameObjectTransform is nullptr!");
			continue;
		}

		// INFO: Continue if the mesh/go is inactive
		if (!mesh->GetIsActive() || !owningGameObject->GetIsActive())
			continue;

		XMMATRIX worldMatrix = owningGameObjectTransform->GetWorldMatrix();

		// INFO: Set the constant buffer properties based on the constant buffer type for each constant buffer found in the material
		for (auto& cb : material->GetConstantBuffers())
		{
			ConstantBufferType constantBufferType = cb.first;
			ID3D11Buffer* constantBuffer = cb.second.buffer;

			switch (constantBufferType)
			{
			case DirectXConfig::ConstantBufferType::LitVS:
			{
				LitVSBuffer litVSBuffer{};

				XMMATRIX inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);
				XMMATRIX transposeWorldMatrix = XMMatrixTranspose(worldMatrix);

				litVSBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;

				litVSBuffer.ambientLightColour = ambientLight.GetColour();

				litVSBuffer.directionalLightColour = directionalLight.GetColour();
				litVSBuffer.directionalLightDirection = XMVector3Transform(directionalLight.GetDirection(), transposeWorldMatrix);

				// INFO: Go through all point lights
				for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
				{
					if (!pointLights[i].GetIsEnabled())
						continue;

					litVSBuffer.pointLights[i].SetColour(pointLights[i].GetColour());
					litVSBuffer.pointLights[i].SetPosition(XMVector3Transform(pointLights[i].GetPosition(), inverseWorldMatrix));
					litVSBuffer.pointLights[i].SetStrength(pointLights[i].GetStrength());
					litVSBuffer.pointLights[i].SetIsEnabled(pointLights[i].GetIsEnabled());
				}

				deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &litVSBuffer, 0, 0);
				break;
			}
			case DirectXConfig::ConstantBufferType::UnlitVS:
			{
				UnlitVSBuffer unlitVSBuffer{};

				unlitVSBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;

				deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &unlitVSBuffer, 0, 0);
				break;
			}
			case DirectXConfig::ConstantBufferType::ReflectiveVS:
			{
				ReflectiveVSBuffer reflectiveVSBuffer{};

				XMMATRIX inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);
				XMMATRIX transposeWorldMatrix = XMMatrixTranspose(worldMatrix);

				reflectiveVSBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;
				reflectiveVSBuffer.wv = worldMatrix * viewMatrix;

				reflectiveVSBuffer.ambientLightColour = ambientLight.GetColour();

				reflectiveVSBuffer.directionalLightColour = directionalLight.GetColour();
				reflectiveVSBuffer.directionalLightDirection = XMVector3Transform(directionalLight.GetDirection(), transposeWorldMatrix);

				// INFO: Go through all point lights
				for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
				{
					if (!pointLights[i].GetIsEnabled())
						continue;

					reflectiveVSBuffer.pointLights[i].SetColour(pointLights[i].GetColour());
					reflectiveVSBuffer.pointLights[i].SetPosition(XMVector3Transform(pointLights[i].GetPosition(), inverseWorldMatrix));
					reflectiveVSBuffer.pointLights[i].SetStrength(pointLights[i].GetStrength());
					reflectiveVSBuffer.pointLights[i].SetIsEnabled(pointLights[i].GetIsEnabled());
				}

				deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &reflectiveVSBuffer, 0, 0);
				break;
			}
			case DirectXConfig::ConstantBufferType::ReflectivePS:
			{
				ReflectivePSBuffer reflectivePSBuffer{};

				reflectivePSBuffer.reflectionAmount = material->GetReflectionAmount();

				deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &reflectivePSBuffer, 0, 0);
				break;
			}
			case DirectXConfig::ConstantBufferType::ParticleVS:
			{
				ParticleVSBuffer particleBuffer{};

				// INFO: Update the world matrix to have the particle face the camera
				worldMatrix = owningGameObject->LookAtXZ(cameraPosition.x, cameraPosition.z) * worldMatrix;

				particleBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;

				// INFO: Set the constant buffer colour based on the particle colour if it is a particle
				if (Particle* particle = dynamic_cast<Particle*>(owningGameObject))
					particleBuffer.colour = particle->GetColour();

				deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &particleBuffer, 0, 0);
				break;
			}
			case DirectXConfig::ConstantBufferType::None:
			default:
				break;
			}
		}

		mesh->Draw(deviceContext.Get());
	}

	// INFO: Render all UI elements
	spriteBatch->Begin();
	for (auto& uiElement : scene->GetUserInterfaceElements())
	{
		if (!uiElement)
		{
			LogWarning("Renderer::RenderFrame(): UI element is nullptr!");
			continue;
		}

		if (!uiElement->GetIsActive())
			continue;

		uiElement->Draw(spriteBatch.get());
	}
	spriteBatch->End();

	swapChain->Present(1, 0);
}
