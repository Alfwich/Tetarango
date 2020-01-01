#include "ParticleHitFleshFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitFlesh.h"

namespace AWGame
{
	ParticleHitFleshFactory::ParticleHitFleshFactory()
	{
		registerGameObject<ParticleHitFleshFactory>("particle-hit-flesh-factory");
	}

	std::shared_ptr<AW::Particle> ParticleHitFleshFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);

		const auto particle = std::make_shared<ParticleHitFlesh>(modules->texture);
		particle->ogW = AW::NumberHelper::random(1, 5);
		particle->ogH = AW::NumberHelper::random(1, 5);
		particle->x = bundle.rect.x;
		particle->y = bundle.rect.y;
		particle->r = randomRotation(rng);
		if (AW::NumberHelper::chance(5))
		{
			particle->state = 1;
			particle->zIndex = -1;
			particle->x = AW::NumberHelper::random(-35, 35);
			particle->y = AW::NumberHelper::random(-35, 35);
			particle->totalLife = 30000.0;
		}
		else
		{
			particle->zIndex = 5;
			particle->ogXV = AW::NumberHelper::random(-100.0, 100.0);
			particle->ogYV = AW::NumberHelper::random(-100.0, 100.0);
			particle->totalLife = 450.0;
		}

		particle->timeLeft = particle->totalLife;

		particle->clip = AW::Rect(
			AW::NumberHelper::random(0, 256),
			AW::NumberHelper::random(0, 256),
			AW::NumberHelper::random(2, 10),
			AW::NumberHelper::random(2, 10)
		);

		return particle;
	}
}

