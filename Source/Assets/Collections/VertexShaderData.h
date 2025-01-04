#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl.h>

struct VertexShaderData
{
public:
	VertexShaderData();
	VertexShaderData(Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout);
	~VertexShaderData();

	inline ID3D11VertexShader* GetVertexShader() const { return vertexShader.Get(); }
	inline ID3D11InputLayout* GetInputLayout() const { return inputLayout.Get(); }

public:
	static const VertexShaderData EMPTY;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};