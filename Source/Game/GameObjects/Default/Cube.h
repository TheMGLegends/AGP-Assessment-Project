#pragma once

#include "../Core/GameObject.h"

class BoxCollider;
class Mesh;

class Cube : public GameObject
{
public:
	Cube();
	virtual ~Cube() override;

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<BoxCollider> boxCollider;
};

