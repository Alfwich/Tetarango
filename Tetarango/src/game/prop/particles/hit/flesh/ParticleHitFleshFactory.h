#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleHitFleshFactory : public AW::IParticleFactory
	{
	public:
		ParticleHitFleshFactory();
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);
	};

}

