#include "BaseProp.h"

#include "Props.h"
#include "GameImports.h"

namespace
{
	const MTGame::Props config;
}

namespace MTGame
{
	bool BaseProp::isLoaded = false;

	void BaseProp::primeProps()
	{
		std::make_unique<TestParticleFactory>();
		std::make_unique<ParticleFiringFactory>();
		std::make_unique<ParticleBulletTrailParticleFactory>();
		std::make_unique<ParticleHitFleshFactory>();
		std::make_unique<ParticleHitHardFactory>();
		std::make_unique<ParticleHitSoftFactory>();
		std::make_unique<ParticleSpaceBackgroundParticleFactory>();
		std::make_unique<BlockParticleFactory>();

		Block::primeBlocks();
	}

	void BaseProp::loadResources(std::shared_ptr<MT::SystemModuleBundle> modules)
	{
		if (isLoaded)
		{
			return;
		}

		modules->texture->loadTexture("res/game/img/prop/particle/particle-hit-soft.png", "particle-hit-soft");
		modules->texture->loadTexture("res/game/img/prop/particle/particle-hit-hard.png", "particle-hit-hard");
		modules->texture->loadTexture("res/game/img/prop/particle/particle-hit-flesh.png", "particle-hit-flesh");
		modules->texture->loadTexture("res/game/img/prop/particle/particle-firing.png", "particle-firing");
		modules->texture->loadTexture("res/game/img/prop/particle/test-particle.png", "test-particle");

		Block::loadResources(modules);

		isLoaded = true;
	}

	const Props& BaseProp::getProps()
	{
		return config;
	}

	BaseProp::BaseProp() :
		propBaseName(""),
		propType(PropType::Unspecified) {};

	BaseProp::BaseProp(PropParticleFactory factory) :
		propBaseName(config.particleFactories.at(factory)),
		propType(PropType::ParticleFactory) {}

	BaseProp::BaseProp(PropBlock block) :
		propBaseName(config.blocks.at(block)),
		propType(PropType::Block) {}
}

