#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ParticleSpaceBackground.h"

namespace AWGame
{
	class ParticleSpaceBackgroundParticleFactory : public MT::IParticleFactory
	{
	public:
		ParticleSpaceBackgroundParticleFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

