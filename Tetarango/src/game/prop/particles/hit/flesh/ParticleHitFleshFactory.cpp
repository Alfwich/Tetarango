#include "ParticleHitFleshFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitFlesh.h"

namespace AWGame
{
	ParticleHitFleshFactory::ParticleHitFleshFactory()
	{
		enableSerialization<ParticleHitFleshFactory>();
	}

	std::shared_ptr<MT::Particle> ParticleHitFleshFactory::makeParticle(MT::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);

		const auto particle = std::make_shared<ParticleHitFlesh>(modules->texture);
		particle->ogW = MT::NumberHelper::random(1, 5);
		particle->ogH = MT::NumberHelper::random(1, 5);
		particle->x = bundle.rect.x;
		particle->y = bundle.rect.y;
		particle->r = randomRotation(rng);
		if (MT::NumberHelper::chance(5))
		{
			particle->state = 1;
			particle->zIndex = -1;
			particle->x = MT::NumberHelper::random(-35, 35);
			particle->y = MT::NumberHelper::random(-35, 35);
			particle->totalLife = 30000.0;
		}
		else
		{
			particle->zIndex = 5;
			particle->ogXV = MT::NumberHelper::random(-100.0, 100.0);
			particle->ogYV = MT::NumberHelper::random(-100.0, 100.0);
			particle->totalLife = 450.0;
		}

		particle->timeLeft = particle->totalLife;

		particle->clip = MT::Rect(
			MT::NumberHelper::random(0, 256),
			MT::NumberHelper::random(0, 256),
			MT::NumberHelper::random(2, 10),
			MT::NumberHelper::random(2, 10)
		);

		return particle;
	}
}

