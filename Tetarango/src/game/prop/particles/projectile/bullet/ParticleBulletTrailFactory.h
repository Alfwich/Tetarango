#pragma once

#include "ui/renderable/primitive/particle/IParticleFactory.h"
#include "ParticleBulletTrail.h"

namespace MTGame
{
	class ParticleBulletTrailParticleFactory : public MT::IParticleFactory
	{
	public:
		ParticleBulletTrailParticleFactory();
		std::shared_ptr<MT::Particle> makeParticle(MT::ParticleCreationBundle& bundle);
	};

}

