#include "VertexShaderData.h"

const VertexShaderData VertexShaderData::EMPTY;

using namespace Microsoft::WRL;

VertexShaderData::VertexShaderData() : vertexShader(nullptr), inputLayout(nullptr)
{
}

VertexShaderData::VertexShaderData(ComPtr<ID3D11VertexShader> _vertexShader, ComPtr<ID3D11InputLayout> _inputLayout) : vertexShader(std::move(_vertexShader)),
																													   inputLayout(std::move(_inputLayout))
{
}

VertexShaderData::~VertexShaderData()
{
}
