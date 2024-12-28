#include "Mesh.h"

#include <iostream>

#include "../../Assets/AssetHandler.h"

Mesh::Mesh(GameObject* _gameObject, std::string modelName, std::string materialName) : Component(_gameObject)
{
	model = AssetHandler::GetModel(modelName);

	if (!model)
		std::cout << "Mesh::Mesh(): Model is nullptr!" << std::endl;

	material = AssetHandler::GetMaterial(materialName);

	if (!material)
		std::cout << "Mesh::Mesh(): Material is nullptr!" << std::endl;
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
