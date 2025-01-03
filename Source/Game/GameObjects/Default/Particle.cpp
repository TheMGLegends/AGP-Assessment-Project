#include "Particle.h"

#include "../../../Components/Mesh/Mesh.h"
#include "../../../Components/Physics/Rigidbody.h"

using namespace DirectX::SimpleMath;

Particle::Particle(const std::string& modelName, const std::string& materialName) : age(0.0f), colour(Vector4::One)
{
	mesh = AddComponent<Mesh>(this, modelName, materialName);
	rigidbody = AddComponent<Rigidbody>(this);

	SetIsActive(false);
}

Particle::~Particle()
{
}
