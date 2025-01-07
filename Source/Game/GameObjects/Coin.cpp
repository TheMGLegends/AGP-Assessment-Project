#include "Coin.h"

#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Physics/SphereCollider.h"
#include "../../Core/Time/Time.h"
#include "../../Utilities/MathUtils.h"

using namespace DirectX::SimpleMath;
using namespace MathUtils;

Coin::Coin() : scoreValue(10), rotationSpeed(1.0f), travelDistance(RandomRange(0.25f, 0.75f))
{
	SetLayer(Layer::Coin);

	mesh = AddComponent<Mesh>(this, "Coin", "CoinMaterial");
	sphereCollider = AddComponent<SphereCollider>(this, 1.75f);

	if (std::shared_ptr<SphereCollider> sc = sphereCollider.lock())
	{
		sc->SetIsTrigger(true);
	}

	if (std::shared_ptr<Transform> t = transform.lock())
	{
		Quaternion randomRotation = Quaternion::CreateFromYawPitchRoll(RandomRange(0.0f, DirectX::XM_2PI), 0.0f, 0.0f);
		t->SetRotation(randomRotation);
	}
}

Coin::~Coin()
{
}

void Coin::Update(float deltaTime)
{
	if (std::shared_ptr<Transform> t = transform.lock())
	{
		t->Rotate(Vector3(0.0f, rotationSpeed * deltaTime, 0.0f));
		t->Translate(Vector3(0.0f, -sinf(Time::GetElapsedTime()) * travelDistance * deltaTime, 0.0f));
	}
}
