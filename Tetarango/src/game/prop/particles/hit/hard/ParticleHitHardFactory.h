#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleHitHardFactory : public MT::IParticleFactory
	{
	public:
		ParticleHitHardFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

