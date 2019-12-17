#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleHitSoftFactory : public AW::IParticleFactory
	{
	public:
		ParticleHitSoftFactory();
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);
	};

}

