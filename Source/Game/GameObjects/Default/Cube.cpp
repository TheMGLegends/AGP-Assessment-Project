#include "Cube.h"

#include "../../../Components/Mesh/Mesh.h"
#include "../../../Components/Physics/BoxCollider.h"

Cube::Cube()
{
	mesh = AddComponent<Mesh>(this, "Cube", "BoxMaterial");
	boxCollider = AddComponent<BoxCollider>(this);
}

Cube::~Cube()
{
}
