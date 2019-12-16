#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ParticleSpaceBackground.h"

namespace AWGame
{
	class ParticleSpaceBackgroundParticleFactory : public AWCore::IParticleFactory
	{
	public:
		ParticleSpaceBackgroundParticleFactory();
		std::shared_ptr<AWCore::Particle> makeParticle(AWCore::ParticleCreationBundle& bundle);
	};

}

