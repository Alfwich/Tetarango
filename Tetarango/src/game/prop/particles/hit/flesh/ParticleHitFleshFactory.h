#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace MTGame
{
	class ParticleHitFleshFactory : public MT::IParticleFactory
	{
	public:
		ParticleHitFleshFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

