#include "GameScene.h"

GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	// INFO: Call the base class initialise
	if (!Scene::Initialise())
		return false;

	// TODO: Initialise the game scene

	return true;
}
