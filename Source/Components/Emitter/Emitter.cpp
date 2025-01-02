#include "Emitter.h"

#include "../Core/ComponentHandler.h"
#include "../Physics/Rigidbody.h"
#include "../../Game/GameObjects/Default/Particle.h"
#include "../../Scene/Core/SceneContext.h"
#include "../../Utilities/MathUtils.h"

using namespace DirectX::SimpleMath;
using namespace MathUtils;

Emitter::Emitter(GameObject* _gameObject, unsigned int _particleCount, 
				 const std::string& modelName, const std::string& materialName, 
				 EffectType _effectType, bool _isLooping, bool _playOnAwake) : Component(_gameObject), particleCount(_particleCount), particlesSpawned(0), 
																			   effectType(_effectType), isLooping(_isLooping), isTriggered(_playOnAwake),
																			   spawnInterval(0.05f), spawnTimer(0.0f)
{
	for (unsigned int i = 0; i < particleCount; ++i)
	{
		// INFO: Register the particles with the scene and add them to the inactive list
		inactiveParticles.emplace_back(SceneContext::SpawnGameObject<Particle>(Vector3::Zero, Quaternion::Identity, modelName, materialName));
	}
}

Emitter::~Emitter()
{
	activeParticles.clear();
	inactiveParticles.clear();
}

void Emitter::Update(float deltaTime)
{
	spawnTimer += deltaTime;

	// INFO: Unique logic for each effect type
	switch (effectType)
	{
	case EffectType::Fountain:
		FountainEffect(deltaTime);
		break;
	case EffectType::Explosion:
		ExplosionEffect(deltaTime);
		break;
	case EffectType::Fire:
		FireEffect(deltaTime);
		break;
	case EffectType::None:
	default:
		break;
	}
}

void Emitter::Play(EffectType _effectType)
{
	// INFO: Set the effect type if the parameter is not none
	if (_effectType != EffectType::None)
		effectType = _effectType;

	isTriggered = true;
	spawnTimer = 0.0f;
	particlesSpawned = 0;
}

void Emitter::FountainEffect(float deltaTime)
{
	std::list<Particle*>::iterator it;

	// INFO: Go through the inactive particles and activate them if it's time and the emitter is triggered
	if (spawnTimer >= spawnInterval && isTriggered)
	{
		spawnTimer = 0.0f;

		it = inactiveParticles.begin();

		// INFO: Given that there are inactive particles, initialise them and move them to the active list
		if (!inactiveParticles.empty())
		{
			(*it)->age = RandomRange(3.0f, 7.5f);
			(*it)->colour = Vector4(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), 1.0f);
			float scale = RandomRange(0.1f, 0.5f);

			if (std::shared_ptr<Rigidbody> rb = (*it)->rigidbody.lock())
				rb->SetVelocity(Vector3(RandomRange(-1.0f, 1.0f), RandomRange(1.5f, 2.5f), RandomRange(-1.0f, 1.0f)));

			if (std::shared_ptr<Transform> transform = GetGameObject()->transform.lock())
			{
				(*it)->transform.lock()->SetPosition(GetGameObject()->transform.lock()->GetPosition());
				(*it)->transform.lock()->SetScale(Vector3(scale, scale, scale));
			}

			(*it)->SetIsActive(true);

			activeParticles.push_back(*it);
			inactiveParticles.pop_front();

			++particlesSpawned;
		}
	}

	// INFO: Go through the active particles and update them (Note: Drawing logic is done in renderer, not here)
	if (!activeParticles.empty())
	{
		for (it = activeParticles.begin(); it != activeParticles.end(); ++it)
		{
			(*it)->age -= deltaTime;

			if ((*it)->age <= 0.0f)
			{
				(*it)->SetIsActive(false);

				inactiveParticles.push_back(*it);
				activeParticles.erase(it++);

				// INFO: If the emitter is not looping and all particles have been spawned, stop the emitter
				if (!isLooping && particlesSpawned >= particleCount)
					isTriggered = false;
			}
		}
	}
}

void Emitter::ExplosionEffect(float deltaTime)
{
}

void Emitter::FireEffect(float deltaTime)
{
}
