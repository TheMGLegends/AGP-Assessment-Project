#pragma once

#include "../Core/Component.h"

#include <d3d11.h>
#include <memory>
#include <string>

class Material;
class Model;

class Mesh : public Component, public std::enable_shared_from_this<Mesh>
{
public:
	Mesh(GameObject* _gameObject, const std::string& modelName, const std::string& materialName);
	virtual ~Mesh() override;

	Material* GetMaterial() const;

	/// @brief Wrapper function for calling model->Draw() and setting the material properties
	void Draw(ID3D11DeviceContext* deviceContext);

private:
	virtual void RegisterComponent() override;

private:
	Model* model;
	Material* material;
};

