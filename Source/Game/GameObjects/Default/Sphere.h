#pragma once

#include "../Core/GameObject.h"

class SphereCollider;
class Mesh;

class Sphere : public GameObject
{
public:
	Sphere();
	virtual ~Sphere() override;

private:
	std::weak_ptr<Mesh> mesh;
	std::weak_ptr<SphereCollider> sphereCollider;
};

