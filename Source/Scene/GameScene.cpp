#include "GameScene.h"

#include <DirectXMath.h>

// TEST CODE
#include "../Game/GameObjects/Player.h"

using namespace DirectX;

GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	bool result = Scene::Initialise();

	// TODO: Initialise the game scene
	// TEST CODE
	gameObjects.emplace_back(std::make_unique<Player>());
	AddPointLight(PointLight(XMVectorSet(0.9f, 0.0f, 0.85f, 1.0f), XMVECTOR{ 1.5f, 0.0f, -1.0f }, 10));

	return result;
}
