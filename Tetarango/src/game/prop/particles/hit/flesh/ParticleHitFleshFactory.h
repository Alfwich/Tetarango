#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleHitFleshFactory : public MT::IParticleFactory
	{
	public:
		ParticleHitFleshFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

