#include "Mesh.h"

#include <iostream>

#include "../../Assets/AssetHandler.h"
#include "../../Assets/Material/Material.h"
#include "../../Assets/Model/Model.h"

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

void Mesh::Draw(ID3D11DeviceContext* deviceContext)
{
	if (!deviceContext)
	{
		std::cout << "Mesh::Draw(): DeviceContext is nullptr!" << std::endl;
		return;
	}

	// INFO: Set the material properties
	if (material)
		material->Set(deviceContext);

	// INFO: Draw the model
	if (model)
		model->Draw();
}
