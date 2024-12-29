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

	// INFO: Validate whether the materials constant buffer is of type Unlit (Skybox)
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

	// INFO: Populate the Unlit (Skybox) constant buffer
	UnlitBuffer unlitBuffer{};
	unlitBuffer.wvp = translationMatrix * viewMatrix * projectionMatrix;

	// INFO: Update the device context sub resource with the Unlit (Skybox) constant buffer
	deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &unlitBuffer, 0, 0);

	// INFO: Draw the skybox (Firstly sets the material properties and then draws the mesh)
	mesh->Draw(deviceContext);
}
