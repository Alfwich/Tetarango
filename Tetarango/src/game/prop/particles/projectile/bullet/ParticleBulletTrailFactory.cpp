#include "ParticleBulletTrailFactory.h"

namespace AWGame
{
	ParticleBulletTrailParticleFactory::ParticleBulletTrailParticleFactory()
	{
		registerGameObject<ParticleBulletTrailParticleFactory>(__FUNCTION__);
	}

	std::shared_ptr<AW::Particle> ParticleBulletTrailParticleFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleBulletTrail>();
		particle->w = bundle.rect.w;
		particle->h = bundle.rect.h;
		particle->ogW = particle->w;
		particle->ogH = particle->h;
		particle->r = bundle.rotation;
		particle->totalLife = 100.0;
		particle->timeLeft = 100.0;
		particle->clip = AW::Rect(0, 0, 1, 1);

		return particle;
	}
}

