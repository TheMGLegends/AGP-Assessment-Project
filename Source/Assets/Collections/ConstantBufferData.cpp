#include "ConstantBufferData.h"

const ConstantBufferData ConstantBufferData::EMPTY;

using namespace DirectXConfig;
using namespace Microsoft::WRL;

ConstantBufferData::ConstantBufferData() : constantBuffer(nullptr), constantBufferType(ConstantBufferType::None)
{
}

ConstantBufferData::ConstantBufferData(ComPtr<ID3D11Buffer> _constantBuffer, ConstantBufferType _constantBufferType) : constantBuffer(std::move(_constantBuffer)), 
																													   constantBufferType(_constantBufferType)
{
}

ConstantBufferData::~ConstantBufferData()
{
}
