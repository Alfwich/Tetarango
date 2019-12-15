#include "ParticleBulletTrailFactory.h"

namespace MTGame
{
	ParticleBulletTrailParticleFactory::ParticleBulletTrailParticleFactory()
	{
		enableSerialization<ParticleBulletTrailParticleFactory>();
	}

	std::shared_ptr<MT::Particle> ParticleBulletTrailParticleFactory::makeParticle(MT::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleBulletTrail>(modules->texture);
		particle->w = bundle.rect.w;
		particle->h = bundle.rect.h;
		particle->ogW = particle->w;
		particle->ogH = particle->h;
		particle->r = bundle.rotation;
		particle->totalLife = 100.0;
		particle->timeLeft = 100.0;
		particle->clip = MT::Rect(0, 0, 1, 1);

		return particle;
	}
}

