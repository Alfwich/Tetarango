#include "ParticleFiringFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleFiring.h"

namespace MTGame
{
	ParticleFiringFactory::ParticleFiringFactory()
	{
		enableSerialization<ParticleFiringFactory>();
	}

	std::shared_ptr<MT::Particle> ParticleFiringFactory::makeParticle(MT::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleFiring>(modules->texture);
		particle->zIndex = 1;
		particle->ogW = MT::NumberHelper::random(10, 20);
		particle->ogH = MT::NumberHelper::random(10, 20);
		particle->x = -particle->ogW / 2.0;
		particle->y = -particle->ogH / 2.0;
		particle->r = MT::NumberHelper::random(0, 359);
		particle->totalLife = 100.0;
		particle->timeLeft = 100.0;
		particle->clip = MT::Rect(
			MT::NumberHelper::random(0, 256),
			MT::NumberHelper::random(0, 128),
			4, 4
		);

		return particle;
	}
}

