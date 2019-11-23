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
		particle->x = MT::NumberHelper::random(bundle.rect.w);
		particle->y = MT::NumberHelper::random(bundle.rect.h);
		particle->w = MT::NumberHelper::random(1, 2);
		particle->h = MT::NumberHelper::random(1, 2);
		particle->shouldDecay = false;

		return particle;
	}
}

