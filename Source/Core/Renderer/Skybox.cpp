#include "Skybox.h"

#include <iostream>

#include "../../Assets/Material/Material.h"
#include "../../Assets/ConstantBuffers/ConstantBuffers.h"
#include "../../Components/Mesh/Mesh.h"

using namespace ConstantBuffers;
using namespace DirectX;
using namespace DirectXConfig;

Skybox::Skybox(const std::string& modelName, const std::string& materialName)
{
	mesh = std::make_unique<Mesh>(nullptr, modelName, materialName);

	if (!mesh)
		std::cout << "Skybox::Skybox(): Mesh is nullptr!" << std::endl;
	else 
		if (mesh->GetMaterial()->GetConstantBufferType() != ConstantBufferType::Unlit)
			std::cout << "Skybox::Skybox(): Material constant buffer type is not Unlit (Skybox)!" << std::endl;
}

Skybox::~Skybox()
{
}

void Skybox::Draw(ID3D11DeviceContext* deviceContext, XMMATRIX translationMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!deviceContext)
	{
		std::cout << "Skybox::Draw(): DeviceContext is nullptr!" << std::endl;
		return;
	}

	UnlitBuffer unlitBuffer{};
	unlitBuffer.wvp = translationMatrix * viewMatrix * projectionMatrix;

	deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &unlitBuffer, 0, 0);

	mesh->Draw(deviceContext);
}
