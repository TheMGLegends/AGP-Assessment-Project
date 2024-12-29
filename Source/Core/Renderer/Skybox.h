#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <string>

class Camera;
class Mesh;

class Skybox
{
public:
	Skybox(const std::string& modelName, const std::string& materialName);
	~Skybox();

	void Draw(ID3D11DeviceContext* deviceContext, Camera* camera);

private:
	std::unique_ptr<Mesh> mesh;
};

