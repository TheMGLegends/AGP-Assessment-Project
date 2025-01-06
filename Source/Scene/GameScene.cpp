#include "GameScene.h"

#include <DirectXMath.h>

#include "../Components/Mesh/Mesh.h"
#include "../Assets/Material/Material.h"
#include "../Game/Camera/Camera.h"
#include "../Game/GameObjects/Player.h"
#include "../Game/GameObjects/Default/Cube.h"
#include "../Game/GameObjects/Default/Sphere.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	bool result = Scene::Initialise();

	// INFO: Player & Camera Setup
	std::unique_ptr<Player> player = std::make_unique<Player>();
	std::shared_ptr<Transform> playerTransform = player->transform.lock();
	playerTransform->SetPosition(XMVECTOR{ 0.0f, 2.5f, 0.0f }, false);

	// INFO: Camera has target and inline with players head
	camera->AddObserver(player.get());
	camera->SetTarget(playerTransform.get());
	camera->SetOffset(XMVECTOR{ 0.0f, 1.25f, 0.0f });

	// INFO: Setup player bindings based on initial isFreeCam state
	player->OnNotifyIsFreeCamChange(camera->GetIsFreeCam());

	gameObjects.push_back(std::move(player));

	// INFO: Floor
	std::unique_ptr<Cube> floor = std::make_unique<Cube>();
	std::shared_ptr<Transform> floorTransform = floor->transform.lock();

	floor->GetComponent<Mesh>().lock()->SetMaterial("FloorMaterial");
	floor->SetLayer(Layer::Ground);
	floorTransform->SetPosition(XMVECTOR{ 0.0f, 0.0f, 0.0f }, false);
	floorTransform->SetScale(XMVECTOR{ 50.0f, 0.25f, 50.0f });

	gameObjects.push_back(std::move(floor));

	// INFO: Reflective Sphere
	gameObjects.emplace_back(std::make_unique<Sphere>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 0.0f, 20.0f, 0.0f }, false);

	// INFO: Boxes
	gameObjects.emplace_back(std::make_unique<Cube>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 0.0f, 2.5f, -15.0f }, false);
	gameObjects.back()->transform.lock()->SetScale(XMVECTOR{ 2.0f, 2.0f, 2.0f });

	std::unique_ptr<Cube> litCube = std::make_unique<Cube>();
	std::shared_ptr<Transform> litCubeTransform = litCube->transform.lock();

	litCube->GetComponent<Mesh>().lock()->SetMaterial("BoxMaterialLit");
	litCubeTransform->SetPosition(XMVECTOR{ -10.0f, 3.25f, 15.0f }, false);
	litCubeTransform->SetScale(XMVECTOR{ 3.0f, 3.0f, 3.0f });
	Quaternion rotation = Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(45.0f), 0.0f, 0.0f);
	litCubeTransform->SetRotation(rotation);

	gameObjects.push_back(std::move(litCube));

	// TODO: Initialise the game scene

	// TEST CODE
	AddPointLight(PointLight(XMVectorSet(0.3f, 0.534f, 0.85f, 1.0f), XMVECTOR{ -10.0f, 5.0f, 15.0f }, 100));

	return result;
}
