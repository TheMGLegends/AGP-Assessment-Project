#include "Player.h"

// TEST CODE
#include "../../Components/Mesh/Mesh.h"

Player::Player()
{
	SetLayer(Layer::Player);

	// TEST CODE
	AddComponent<Mesh>(this, "Cube", "TestMaterial");
}

Player::~Player()
{
}

void Player::OnCollision(std::shared_ptr<Collider> other)
{
}
