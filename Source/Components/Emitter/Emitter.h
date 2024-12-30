#pragma once

#include "../Core/Component.h"

#include <DirectXMath.h>
#include <list>
#include <memory>
#include <string>

class Particle;

enum class EffectType
{
	None = 0,

	Fountain,
	Explosion,
	Fire
};

class Emitter : public Component, public std::enable_shared_from_this<Emitter>
{
public:
	Emitter(GameObject* _gameObject, unsigned int particleCount, const std::string& modelName, 
			const std::string& materialName, EffectType _effectType = EffectType::Fountain,
			bool _isLooping = true, bool _playOnAwake = true);
	virtual ~Emitter() override;

	virtual void Update(float deltaTime) override;

	inline void SetIsLooping(bool _isLooping) { isLooping = _isLooping; }
	inline bool GetIsLooping() const { return isLooping; }

	/// @brief Effect type none plays the effect type that was set in the constructor
	void Play(EffectType _effectType = EffectType::None);
	inline bool GetIsTriggered() const { return isTriggered; }

private:
	virtual void RegisterComponent() override;

private:
	EffectType effectType;
	bool isLooping;
	bool isTriggered;

	std::list<Particle*> activeParticles;
	std::list<Particle*> inactiveParticles;
};

