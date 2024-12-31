#include "Mesh.h"

#include "../../Assets/AssetHandler.h"
#include "../../Assets/Material/Material.h"
#include "../../Assets/Model/Model.h"
#include "../../Components/Core/ComponentHandler.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace DebugUtils;

Mesh::Mesh(GameObject* _gameObject, const std::string& modelName, const std::string& materialName) : Component(_gameObject)
{
	model = AssetHandler::GetModel(modelName);

	if (!model)
		LogWarning("Mesh::Mesh(): Model is nullptr! Model name: " + modelName);

	material = AssetHandler::GetMaterial(materialName);

	if (!material)
		LogWarning("Mesh::Mesh(): Material is nullptr! Material name: " + materialName);
}

Mesh::~Mesh()
{
}

Material* Mesh::GetMaterial() const
{
	return material;
}

void Mesh::Draw(ID3D11DeviceContext* deviceContext)
{
	if (!deviceContext)
	{
		LogError("Mesh::Draw(): DeviceContext is nullptr!");
		return;
	}

	if (material)
		material->Set(deviceContext);

	if (model)
		model->Draw();
}
