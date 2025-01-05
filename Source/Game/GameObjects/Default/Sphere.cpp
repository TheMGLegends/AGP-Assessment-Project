#include "Sphere.h"

#include "../../../Components/Mesh/Mesh.h"
#include "../../../Components/Physics/SphereCollider.h"

Sphere::Sphere()
{
	// TODO: Add proper material
	mesh = AddComponent<Mesh>(this, "Sphere", "TestMaterial");
	sphereCollider = AddComponent<SphereCollider>(this, 3.0f);
}

Sphere::~Sphere()
{
}
