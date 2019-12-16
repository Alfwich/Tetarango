#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ParticleBulletTrail.h"

namespace AWGame
{
	class ParticleBulletTrailParticleFactory : public AWCore::IParticleFactory
	{
	public:
		ParticleBulletTrailParticleFactory();
		std::shared_ptr<AWCore::Particle> makeParticle(AWCore::ParticleCreationBundle& bundle);
	};

}

