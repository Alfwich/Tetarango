#include "ParticleHitHardFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitHard.h"

namespace MTGame
{
	ParticleHitHardFactory::ParticleHitHardFactory()
	{
		enableSerialization<ParticleHitHardFactory>();
	}

	std::shared_ptr<MT::Particle> ParticleHitHardFactory::makeParticle(MT::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);
		std::uniform_int_distribution<std::mt19937::result_type> randomClip(0, 64);

		const auto particle = std::make_shared<ParticleHitHard>(modules->texture);
		particle->x = MT::NumberHelper::random(-bundle.rect.w, bundle.rect.w);
		particle->y = MT::NumberHelper::random(-bundle.rect.h, bundle.rect.h);
		particle->ogW = MT::NumberHelper::random(2, 6);
		particle->ogH = MT::NumberHelper::random(2, 6);
		particle->r = randomRotation(rng);
		particle->ogXV = MT::NumberHelper::random(-25, 25);
		particle->ogYV = MT::NumberHelper::random(-25, 25);
		particle->totalLife = 250.0;
		particle->timeLeft = 250.0;
		particle->clip = MT::Rect(
			MT::NumberHelper::random(0, 256),
			MT::NumberHelper::random(0, 256),
			particle->ogW,
			particle->ogH
		);

		return particle;
	}
}

