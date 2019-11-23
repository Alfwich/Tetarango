#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace MTGame
{
	class ParticleFiringFactory : public MT::IParticleFactory
	{
	public:
		ParticleFiringFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

