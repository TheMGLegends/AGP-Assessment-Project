#pragma once

#include "../Core/GameObject.h"

#include <string>
#include <SimpleMath.h>

class Mesh;
class Rigidbody;

class Particle : public GameObject
{
	friend class Emitter;

public:
	Particle(const std::string& modelName, const std::string& materialName);
	virtual ~Particle() override;

	DirectX::SimpleMath::Vector4 GetColour() const { return colour; }

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<Rigidbody> rigidbody;

	float age;
	DirectX::SimpleMath::Vector4 colour;
};

