#include "Sphere.h"

#include "../../../Components/Mesh/Mesh.h"
#include "../../../Components/Physics/SphereCollider.h"

#include <DirectXMath.h>

Sphere::Sphere()
{
	// TODO: Add proper material
	mesh = AddComponent<Mesh>(this, "Sphere", "TestMaterial");
	sphereCollider = AddComponent<SphereCollider>(this, 3.0f);
}

Sphere::~Sphere()
{
}

void Sphere::Update(float deltaTime)
{
	// TEST CODE
	//DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.5f * deltaTime);
	//transform.lock()->Translate(position);
}
