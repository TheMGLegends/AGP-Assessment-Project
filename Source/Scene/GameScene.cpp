#include "GameScene.h"

#include <DirectXMath.h>

#include "../Components/Mesh/Mesh.h"
#include "../Assets/Material/Material.h"
#include "../Game/Camera/Camera.h"
#include "../Game/GameObjects/Player.h"
#include "../Game/GameObjects/Coin.h"
#include "../Game/GameObjects/Default/Cube.h"
#include "../Game/GameObjects/Default/Sphere.h"
#include "../UI/FPSCounter.h"
#include "../UI/ScoreCounter.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

GameScene::~GameScene()
{
}

bool GameScene::Initialise()
{
	bool result = Scene::Initialise();

	// INFO: Score Counter
	std::unique_ptr<ScoreCounter> scoreCounter = std::make_unique<ScoreCounter>("SimpleFont", Vector2(0, 0));
	scoreCounter->SetText("Score: 0");

	// INFO: FPS Counter
	std::unique_ptr<FPSCounter> fpsCounter = std::make_unique<FPSCounter>("SimpleFont", Vector2(0, 20));
	fpsCounter->SetText("FPS: 0");
	uiElements.emplace_back(std::move(fpsCounter));

	// INFO: Tooltip Text
	std::unique_ptr<UserInterfaceElement> tooltipText = std::make_unique<UserInterfaceElement>("SimpleFont", Vector2(595, 0), Colors::White);
	tooltipText->SetText("M = Change Mouse Mode\nR = Reset Camera\nF1 = Change Debug Mode\nF2 = Possess Player\nF3 = Change VSync");
	tooltipText->SetScale(Vector2(0.35f, 0.35f));
	uiElements.emplace_back(std::move(tooltipText));

	// INFO: Player & Camera Setup
	std::unique_ptr<Player> player = std::make_unique<Player>();
	player->AddObserver(scoreCounter.get());
	std::shared_ptr<Transform> playerTransform = player->transform.lock();
	playerTransform->SetPosition(XMVECTOR{ 0.0f, 2.5f, 0.0f }, false);
	
	uiElements.push_back(std::move(scoreCounter));

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

	// INFO: Sphere
	std::unique_ptr<Sphere> litSphere = std::make_unique<Sphere>();
	std::shared_ptr<Transform> litSphereTransform = litSphere->transform.lock();

	litSphere->GetComponent<Mesh>().lock()->SetMaterial("BoxMaterialLit");
	litSphereTransform->SetPosition(XMVECTOR{ 10.0f, 3.25f, 15.0f }, false);
	gameObjects.push_back(std::move(litSphere));

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

	// INFO: Coins
	gameObjects.emplace_back(std::make_unique<Coin>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 10.0f, 2.5f, 0.0f }, false);
	gameObjects.emplace_back(std::make_unique<Coin>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 0.0f, 2.5f, 10.0f }, false);
	gameObjects.emplace_back(std::make_unique<Coin>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ -10.0f, 2.5f, 0.0f }, false);
	gameObjects.emplace_back(std::make_unique<Coin>());
	gameObjects.back()->transform.lock()->SetPosition(XMVECTOR{ 0.0f, 2.5f, -10.0f }, false);

	// TODO: Initialise the game scene

	// TEST CODE
	AddPointLight(PointLight(XMVectorSet(0.3f, 0.534f, 0.85f, 1.0f), XMVECTOR{ -10.0f, 5.0f, 15.0f }, 100));

	return result;
}
