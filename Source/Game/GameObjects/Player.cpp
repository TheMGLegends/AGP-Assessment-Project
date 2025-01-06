#include "Player.h"

#include "../../Components/Mesh/Mesh.h"
#include "../../Components/Physics/BoxCollider.h"
#include "../../Components/Physics/Rigidbody.h"

using namespace DirectX::SimpleMath;

Player::Player()
{
	SetLayer(Layer::Player);

	mesh = AddComponent<Mesh>(this, "Cylinder", "PlayerMaterial");
	boxCollider = AddComponent<BoxCollider>(this);
	rigidbody = AddComponent<Rigidbody>(this);

	boxCollider.lock()->SetOffsetScale(Vector3(0.5f, 1.0f, 0.5f));
	rigidbody.lock()->SetUseGravity(false);
}

Player::~Player()
{
}

void Player::OnNotifyIsFreeCamChange(bool isFreeCam)
{
	// TODO: Implement the unbinding and binding of input keys here based on whether the camera is free or not
}
