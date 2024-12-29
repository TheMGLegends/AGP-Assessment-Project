#include "GameScene.h"

// TEST CODE
#include "../Game/GameObjects/Player.h"

GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	// INFO: Call the base class initialise
	if (!Scene::Initialise())
		return false;

	// TODO: Initialise the game scene
	// TEST CODE
	gameObjects.emplace_back(std::make_unique<Player>());

	return true;
}
