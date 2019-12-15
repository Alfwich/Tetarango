#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleHitSoftFactory : public MT::IParticleFactory
	{
	public:
		ParticleHitSoftFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

