#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ParticleSpaceBackground.h"

namespace AWGame
{
	class ParticleSpaceBackgroundParticleFactory : public AW::IParticleFactory
	{
	public:
		ParticleSpaceBackgroundParticleFactory();
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);
	};

}

