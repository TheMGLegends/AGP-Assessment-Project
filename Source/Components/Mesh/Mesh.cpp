#include "Mesh.h"

#include <iostream>

#include "../../Assets/AssetHandler.h"
#include "../../Assets/Material/Material.h"
#include "../../Assets/Model/Model.h"
#include "../../Components/Core/ComponentHandler.h"

Mesh::Mesh(GameObject* _gameObject, const std::string& modelName, const std::string& materialName) : Component(_gameObject)
{
	model = AssetHandler::GetModel(modelName);

	if (!model)
		std::cout << "Mesh::Mesh(): Model is nullptr! Model name: " << modelName << std::endl;

	material = AssetHandler::GetMaterial(materialName);

	if (!material)
		std::cout << "Mesh::Mesh(): Material is nullptr! Material name: " << materialName << std::endl;
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
		std::cout << "Mesh::Draw(): DeviceContext is nullptr!" << std::endl;
		return;
	}

	if (material)
		material->Set(deviceContext);

	if (model)
		model->Draw();
}
