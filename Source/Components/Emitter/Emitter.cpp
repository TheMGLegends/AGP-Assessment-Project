#include "Emitter.h"

#include <iostream>

#include "../Core/ComponentHandler.h"
#include "../../Game/GameObjects/Default/Particle.h"
#include "../../Scene/Core/SceneContext.h"

using namespace DirectX::SimpleMath;

Emitter::Emitter(GameObject* _gameObject, unsigned int particleCount, 
				 const std::string& modelName, const std::string& materialName, 
				 EffectType _effectType, bool _isLooping, bool _playOnAwake) : Component(_gameObject), effectType(_effectType), 
																			   isLooping(_isLooping), isTriggered(_playOnAwake)
{
	// INFO: Get a reference to the owning game object's transform
	std::shared_ptr<Transform> transform = GetGameObject()->transform.lock();

	// INFO: Create the particles
	for (unsigned int i = 0; i < particleCount; ++i)
	{
		// INFO: Register the particles with the scene and add them to the inactive list
		inactiveParticles.emplace_back(SceneContext::SpawnGameObject<Particle>(transform->GetPosition(), transform->GetRotation(), modelName, materialName));
	}
}

Emitter::~Emitter()
{
}

void Emitter::Update(float deltaTime)
{
	// TODO: Update logic here for both inactive and active particles
}

void Emitter::Play(EffectType _effectType)
{
	// INFO: Set the effect type if it is not none
	if (_effectType != EffectType::None)
		effectType = _effectType;

	isTriggered = true;
}

void Emitter::RegisterComponent()
{
	// INFO: Register the mesh component to the mesh vector in the ComponentHandler
	ComponentHandler::emitters.push_back(shared_from_this());
}
