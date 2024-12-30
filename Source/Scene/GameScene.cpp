#include "GameScene.h"

// TEST CODE
#include "../Game/GameObjects/Player.h"


GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	bool result = Scene::Initialise();

	// TODO: Initialise the game scene
	// TEST CODE
	gameObjects.emplace_back(std::make_unique<Player>());

	return result;
}
