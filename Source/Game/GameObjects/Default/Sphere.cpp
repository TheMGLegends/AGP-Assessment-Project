#include "Sphere.h"

#include "../../../Components/Mesh/Mesh.h"
#include "../../../Components/Physics/SphereCollider.h"

#include <DirectXMath.h>

Sphere::Sphere()
{
	mesh = AddComponent<Mesh>(this, "Sphere", "SphereMaterial");
	sphereCollider = AddComponent<SphereCollider>(this, 3.0f);
}

Sphere::~Sphere()
{
}
