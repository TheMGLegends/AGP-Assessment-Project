#pragma once

#include "Core/Scene.h"

class GameScene : public Scene
{
public:
	using Scene::Scene;
	virtual ~GameScene() override;

	virtual bool Initialise() override;
};

