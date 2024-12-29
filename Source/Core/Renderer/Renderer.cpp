#include "Renderer.h"

#include <iostream>
#include <memory>

#include "../../Assets/AssetHandler.h"
#include "../../Assets/Config/DirectXConfig.h"
#include "../../Assets/ConstantBuffers/ConstantBuffers.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Scene/Core/Scene.h"

using namespace ConstantBuffers;
using namespace DirectX;
using namespace DirectXConfig;
using namespace Microsoft::WRL;

Renderer::Renderer() : device(nullptr), deviceContext(nullptr), swapChain(nullptr), 
					   renderTargetView(nullptr), depthStencilView(nullptr), viewport({ 0 }), 
					   spriteBatch(nullptr)
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
		std::cout << "Renderer::Initialise(): Failed to create device and swapchain!" << std::endl;
		return hResult;
	}

	// INFO: Get the back buffer
	ComPtr<ID3D11Resource> backBuffer;

	hResult = swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);

	if (FAILED(hResult))
	{
		std::cout << "Renderer::Initialise(): Failed to get back buffer!" << std::endl;
		return hResult;
	}

	// INFO: Create the render target view
	hResult = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView);

	if (FAILED(hResult))
	{
		std::cout << "Renderer::Initialise(): Failed to create render target view!" << std::endl;
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
		std::cout << "Renderer::Initialise(): Failed to create depth stencil buffer!" << std::endl;
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
		std::cout << "Renderer::Initialise(): Failed to create depth stencil view!" << std::endl;
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

	if (spriteBatch == nullptr)
	{
		std::cout << "Renderer::Initialise(): Failed to create sprite batch!" << std::endl;
		return E_FAIL;
	}

	// INFO: Initialise the AssetHandler
	hResult = AssetHandler::Initialise(device.Get(), deviceContext.Get());

	if (FAILED(hResult))
	{
		std::cout << "Renderer::Initialise(): Failed to initialise the AssetHandler!" << std::endl;
		return hResult;
	}

	// INFO: Load all assets via the AssetHandler
	hResult = AssetHandler::LoadAssets();

	if (FAILED(hResult))
	{
		std::cout << "Renderer::Initialise(): Failed to load assets!" << std::endl;
		return hResult;
	}

	return S_OK;
}

void Renderer::RenderFrame(Scene* scene)
{
	// INFO: Clear the render target view and depth stencil view
	deviceContext->ClearRenderTargetView(renderTargetView.Get(), Colors::DarkSeaGreen);
	deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// INFO: Set the primitive topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// INFO: Get all the properties we need
	Camera* camera = scene->camera.get();
	Skybox* skybox = scene->skybox.get();

	XMMATRIX viewMatrix = camera->GetViewMatrix();
	XMMATRIX projectionMatrix = camera->GetProjectionMatrix();

	// INFO: Render the skybox
	if (skybox)
		skybox->Draw(deviceContext.Get(), camera);

	// TODO: MIGHT BE BETTER TO STORE ALL MESH IN COMPONENT HANDLER AND ACCESS THIS INSTEAD OF GET COMPONENT

	// INFO: Render all game objects
	for (auto& gameObject : scene->gameObjects)
	{
		std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Mesh>().lock();

		// INFO: Continue if the game object doesn't have a mesh component or the game object is inactive
		if (!mesh || !gameObject->GetIsActive())
			continue;

		// INFO: Get the game object's world matrix
		XMMATRIX worldMatrix = gameObject->transform.lock()->GetWorldMatrix();

		switch (mesh->GetMaterial()->GetConstantBufferType())
		{
		case ConstantBufferType::Lit:
		{
			LitBuffer litBuffer{};

			litBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;

			// TODO: Set the light properties

			deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &litBuffer, 0, 0);

			break;
		}
		case ConstantBufferType::Unlit:
		{
			UnlitBuffer unlitBuffer{};

			unlitBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;

			deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &unlitBuffer, 0, 0);

			break;
		}
		case ConstantBufferType::Reflective:
		{
			ReflectiveBuffer reflectiveBuffer{};

			reflectiveBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;
			reflectiveBuffer.wv = worldMatrix * viewMatrix;

			// TODO: Set the light properties

			deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &reflectiveBuffer, 0, 0);

			break;
		}
		case ConstantBufferType::Particle:
		{
			ParticleBuffer particleBuffer{};

			particleBuffer.wvp = worldMatrix * viewMatrix * projectionMatrix;

			// TODO: Set the colour property

			deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &particleBuffer, 0, 0);

			break;
		}
		case ConstantBufferType::None:
		default:
			break;
		}

		// INFO: Draw the mesh (Sets the material properties and then draws the mesh)
		mesh->Draw(deviceContext.Get());
	}

	// INFO: Render all UI elements
	spriteBatch->Begin();
	for (auto& uiElement : scene->uiElements)
	{
		// INFO: Continue if the UI element is inactive
		if (!uiElement->GetIsActive())
			continue;

		// INFO: Draw the UI element
		uiElement->Draw(spriteBatch.get());
	}
	spriteBatch->End();

	// INFO: Present the back buffer
	swapChain->Present(1, 0);
}
