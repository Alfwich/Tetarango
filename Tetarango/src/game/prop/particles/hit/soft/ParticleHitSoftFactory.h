#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"

namespace AWGame
{
	class ParticleHitSoftFactory : public AWCore::IParticleFactory
	{
	public:
		ParticleHitSoftFactory();
		std::shared_ptr<AWCore::Particle> makeParticle(AWCore::ParticleCreationBundle& bundle);
	};

}

