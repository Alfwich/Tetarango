#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace MTGame
{
	class ParticleHitHardFactory : public MT::IParticleFactory
	{
	public:
		ParticleHitHardFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

