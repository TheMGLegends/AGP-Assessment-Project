#include "GameScene.h"

#include <DirectXMath.h>

// TEST CODE
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

	std::unique_ptr<Player> player = std::make_unique<Player>();
	std::shared_ptr<Transform> playerTransform = player->transform.lock();
	playerTransform->Translate(XMVECTOR{ 0.0f, 0.0f, 5.0f });

	// INFO: Camera has target and inline with players head
	camera->AddObserver(player.get());
	camera->SetTarget(playerTransform.get());
	camera->SetOffset(XMVECTOR{ 0.0f, 1.25f, 0.0f });

	gameObjects.push_back(std::move(player));

	// TODO: Initialise the game scene
	// TEST CODE

	gameObjects.emplace_back(std::make_unique<Sphere>());
	gameObjects.back()->transform.lock()->Translate(XMVECTOR{ 0.0f, 0.0f, -5.0f });
	gameObjects.emplace_back(std::make_unique<Cube>());
	gameObjects.back()->transform.lock()->Translate(XMVECTOR{ 0.0f, 0.0f, -15.0f });
	AddPointLight(PointLight(XMVectorSet(0.9f, 0.0f, 0.85f, 1.0f), XMVECTOR{ 1.5f, 0.0f, -1.0f }, 10));

	return result;
}
