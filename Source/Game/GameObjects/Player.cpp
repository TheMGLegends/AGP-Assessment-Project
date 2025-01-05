#include "Player.h"

// TEST CODE
#include "../../Components/Mesh/Mesh.h"

Player::Player()
{
	SetLayer(Layer::Player);

	// TEST CODE
	AddComponent<Mesh>(this, "Cube", "TestMaterial");
	//transform.lock()->SetScale(DirectX::SimpleMath::Vector3{ 1.0f, 1.0f, 1.0f });
}

Player::~Player()
{
}

void Player::OnCollision(std::shared_ptr<Collider> other)
{
}
