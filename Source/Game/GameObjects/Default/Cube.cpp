#include "Cube.h"

#include "../../../Components/Mesh/Mesh.h"
//#include "../../../Components/Physics/BoxCollider.h"

Cube::Cube()
{
	// TODO: Add proper material
	mesh = AddComponent<Mesh>(this, "Cube", "TestMaterial");
	//boxCollider = AddComponent<BoxCollider>(this);
}
