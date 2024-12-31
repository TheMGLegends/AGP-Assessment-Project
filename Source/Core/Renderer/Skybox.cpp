#include "Skybox.h"

#include "../../Assets/Material/Material.h"
#include "../../Assets/ConstantBuffers/ConstantBuffers.h"
#include "../../Components/Mesh/Mesh.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace ConstantBuffers;
using namespace DebugUtils;
using namespace DirectX;
using namespace DirectXConfig;

Skybox::Skybox(const std::string& modelName, const std::string& materialName)
{
	mesh = std::make_unique<Mesh>(nullptr, modelName, materialName);

	if (!mesh)
		LogWarning("Skybox::Skybox(): Mesh is nullptr!");
	else 
		if (mesh->GetMaterial()->GetConstantBufferType() != ConstantBufferType::Unlit)
			LogWarning("Skybox::Skybox(): Material constant buffer type is not Unlit (Skybox)!");
}

Skybox::~Skybox()
{
}

void Skybox::Draw(ID3D11DeviceContext* deviceContext, XMMATRIX translationMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!deviceContext)
	{
		LogError("Skybox::Draw(): DeviceContext is nullptr!");
		return;
	}

	UnlitBuffer unlitBuffer{};
	unlitBuffer.wvp = translationMatrix * viewMatrix * projectionMatrix;

	deviceContext->UpdateSubresource(mesh->GetMaterial()->GetConstantBuffer(), 0, nullptr, &unlitBuffer, 0, 0);

	mesh->Draw(deviceContext);
}
