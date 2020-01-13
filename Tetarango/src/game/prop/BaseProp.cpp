#include "BaseProp.h"

#include "Props.h"

namespace
{
	const AWGame::Props config;
}

namespace AWGame
{
	const Props& BaseProp::getProps()
	{
		return config;
	}

	BaseProp::BaseProp() :
		propBaseName(""),
		propType(PropType::Unspecified)
	{};

	BaseProp::BaseProp(PropParticleFactory factory) :
		propBaseName(config.particleFactories.at(factory)),
		propType(PropType::ParticleFactory)
	{}

	BaseProp::BaseProp(PropBlock block) :
		propBaseName(config.blocks.at(block)),
		propType(PropType::Block)
	{}
}

