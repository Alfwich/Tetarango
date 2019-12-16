#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleFiringFactory : public AWCore::IParticleFactory
	{
	public:
		ParticleFiringFactory();
		std::shared_ptr<AWCore::Particle> makeParticle(AWCore::ParticleCreationBundle& bundle);
	};

}

