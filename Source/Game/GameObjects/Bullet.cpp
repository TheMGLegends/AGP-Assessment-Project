#include "Bullet.h"

#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Physics/SphereCollider.h"

#include <DirectXMath.h>

using namespace DirectX::SimpleMath;

Bullet::Bullet() : movementSpeed(50.0f), lifetimeDuration(5.0f), lifetimeTimer(0.0f)
{
	mesh = AddComponent<Mesh>(this, "Sphere", "BulletMaterial");
	sphereCollider = AddComponent<SphereCollider>(this, 0.1f);

	if (std::shared_ptr<SphereCollider> sc = sphereCollider.lock())
	{
		sc->SetIsTrigger(true);
	}

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		t->SetScale(Vector3(0.025f, 0.025f, 0.025f));
	}
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime)
{
	lifetimeTimer += deltaTime;

	if (lifetimeTimer >= lifetimeDuration)
	{
		Destroy();
	}

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		t->Translate(t->GetForwardVector() * movementSpeed * deltaTime);
	}
}

void Bullet::OnTrigger(std::shared_ptr<Collider> other)
{
	if (GameObject* otherGameObject = other->GetGameObject())
	{
		if (otherGameObject->GetLayer() == Layer::Default)
		{
			otherGameObject->Destroy();
			Destroy();
		}
		else if (otherGameObject->GetLayer() == Layer::Ground)
		{
			Destroy();
		}
	}
}
