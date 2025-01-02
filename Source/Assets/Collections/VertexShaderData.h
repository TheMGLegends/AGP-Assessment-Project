#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl.h>

struct VertexShaderData
{
public:
	VertexShaderData() : vertexShader(nullptr), inputLayout(nullptr) {}
	VertexShaderData(Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout)
	{
		vertexShader = std::move(_vertexShader);
		inputLayout = std::move(_inputLayout);
	}
	~VertexShaderData() = default;

	inline ID3D11VertexShader* GetVertexShader() const { return vertexShader.Get(); }
	inline ID3D11InputLayout* GetInputLayout() const { return inputLayout.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};