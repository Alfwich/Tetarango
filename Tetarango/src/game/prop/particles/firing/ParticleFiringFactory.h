#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleFiringFactory : public AW::IParticleFactory
	{
	public:
		ParticleFiringFactory();
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);
	};

}

