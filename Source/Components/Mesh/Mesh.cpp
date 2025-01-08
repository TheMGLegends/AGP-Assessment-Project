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

	SetMaterial(materialName);
}

Mesh::~Mesh()
{
}

void Mesh::SetModel(const std::string& modelName)
{
	model = AssetHandler::GetModel(modelName);

	if (!model)
		LogWarning("Mesh::SetModel(): Model is nullptr! Model name: " + modelName);
}

Model* Mesh::GetModel() const
{
	return model;
}

void Mesh::SetMaterial(const std::string& materialName)
{
	material = AssetHandler::GetMaterial(materialName);

	if (!material)
		LogWarning("Mesh::SetMaterial(): Material is nullptr! Material name: " + materialName);
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
