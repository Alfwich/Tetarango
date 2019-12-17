#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ParticleBulletTrail.h"

namespace AWGame
{
	class ParticleBulletTrailParticleFactory : public AW::IParticleFactory
	{
	public:
		ParticleBulletTrailParticleFactory();
		std::shared_ptr<AW::Particle> makeParticle(AW::ParticleCreationBundle& bundle);
	};

}

