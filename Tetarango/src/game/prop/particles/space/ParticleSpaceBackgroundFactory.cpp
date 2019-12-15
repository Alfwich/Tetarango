#include "ParticleSpaceBackgroundFactory.h"

#include "util/NumberHelper.h"

namespace MTGame
{
	ParticleSpaceBackgroundParticleFactory::ParticleSpaceBackgroundParticleFactory()
	{
		enableSerialization<ParticleSpaceBackgroundParticleFactory>();
	}

	std::shared_ptr<MT::Particle> ParticleSpaceBackgroundParticleFactory::makeParticle(MT::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleSpaceBackground>(modules->texture);
		particle->zIndex = -1;
		const auto r = MT::NumberHelper::random(0, MT::NumberHelper::PI * 2.0);
		const auto rV = MT::NumberHelper::random(10, 45);
		particle->alphaMod = 0;
		particle->xV = std::cos(r) * rV;
		particle->yV = std::sin(r) * rV;
		particle->x = MT::NumberHelper::random(bundle.rect.w);
		particle->y = MT::NumberHelper::random(bundle.rect.h);
		particle->ogW = MT::NumberHelper::random(1, 4);
		particle->ogH = MT::NumberHelper::random(1, 4);
		particle->timeLeft = 20000.0;
		particle->totalLife = 20000.0;

		return particle;
	}
}

