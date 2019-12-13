#include "BaseProp.h"

#include "Props.h"
#include "GameImports.h"

namespace
{
	const MTGame::Props config;
}

namespace MTGame
{
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
		std::make_unique<Block>();
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

