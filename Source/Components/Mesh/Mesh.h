#pragma once

#include "../Core/Component.h"

#include <d3d11.h>
#include <string>

class Material;
class Model;

class Mesh : public Component
{
public:
	Mesh(GameObject* _gameObject, const std::string& modelName, const std::string& materialName);
	virtual ~Mesh() override;

	inline Material* GetMaterial() const { return material; }

	/// @brief Wrapper function for calling model->Draw() and setting the material properties
	void Draw(ID3D11DeviceContext* deviceContext);

private:
	Model* model;
	Material* material;
};

