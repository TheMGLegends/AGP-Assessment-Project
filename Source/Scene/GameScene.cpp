#include "GameScene.h"

#include <DirectXMath.h>

#include "../Components/Mesh/Mesh.h"
#include "../Assets/Material/Material.h"
#include "../Game/Camera/Camera.h"
#include "../Game/GameObjects/Player.h"
#include "../Game/GameObjects/Default/Cube.h"
#include "../Game/GameObjects/Default/Sphere.h"

using namespace DirectX;

GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	bool result = Scene::Initialise();

	// INFO: Player & Camera Setup
	std::unique_ptr<Player> player = std::make_unique<Player>();
	std::shared_ptr<Transform> playerTransform = player->transform.lock();
	playerTransform->SetPosition(XMVECTOR{ 0.0f, 0.0f, 5.0f }, false);

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
	floorTransform->SetPosition(XMVECTOR{ 0.0f, -5.0f, 0.0f }, false);
	floorTransform->SetScale(XMVECTOR{ 50.0f, 0.25f, 50.0f });

	gameObjects.push_back(std::move(floor));

	// TODO: Initialise the game scene
	// TEST CODE

	gameObjects.emplace_back(std::make_unique<Sphere>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 0.0f, 0.0f, -5.0f }, false);
	gameObjects.emplace_back(std::make_unique<Cube>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 0.0f, 0.0f, -15.0f }, false);
	AddPointLight(PointLight(XMVectorSet(0.9f, 0.0f, 0.85f, 1.0f), XMVECTOR{ 1.5f, 0.0f, -1.0f }, 10));

	return result;
}
