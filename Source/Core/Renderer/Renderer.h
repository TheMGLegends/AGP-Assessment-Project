#pragma once

#include <d3d11.h>
#include <SpriteBatch.h>
#include <Windows.h>
#include <wrl.h>

class Scene;

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	HRESULT Initialise(HWND hWnd);

	void RenderFrame(Scene* scene);

	inline ID3D11Device* GetDevice() const { return device.Get(); }
	inline ID3D11DeviceContext* GetDeviceContext() const { return deviceContext.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	D3D11_VIEWPORT viewport;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
};

