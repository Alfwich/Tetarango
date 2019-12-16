#include "ParticleBulletTrailFactory.h"

namespace AWGame
{
	ParticleBulletTrailParticleFactory::ParticleBulletTrailParticleFactory()
	{
		enableSerialization<ParticleBulletTrailParticleFactory>();
	}

	std::shared_ptr<AWCore::Particle> ParticleBulletTrailParticleFactory::makeParticle(AWCore::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleBulletTrail>(modules->texture);
		particle->w = bundle.rect.w;
		particle->h = bundle.rect.h;
		particle->ogW = particle->w;
		particle->ogH = particle->h;
		particle->r = bundle.rotation;
		particle->totalLife = 100.0;
		particle->timeLeft = 100.0;
		particle->clip = AWCore::Rect(0, 0, 1, 1);

		return particle;
	}
}

