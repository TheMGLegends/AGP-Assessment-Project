#pragma once

#include <d3d11.h>
#include <memory>
#include <SpriteBatch.h>
#include <Windows.h>
#include <wrl.h>

#include "../../Observer/IObserver.h"

class Scene;

class Renderer : public IObserver
{
public:
	Renderer();
	~Renderer();

	HRESULT Initialise(HWND hWnd);

	void RenderFrame(Scene* scene);

	inline ID3D11Device* GetDevice() const { return device.Get(); }
	inline ID3D11DeviceContext* GetDeviceContext() const { return deviceContext.Get(); }

public:
	virtual void OnNotifyWindowSizeChanged(int width, int height) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;

	D3D11_VIEWPORT viewport;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
};

