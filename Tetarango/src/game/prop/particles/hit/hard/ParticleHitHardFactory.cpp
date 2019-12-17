#include "ParticleHitHardFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitHard.h"

namespace AWGame
{
	ParticleHitHardFactory::ParticleHitHardFactory()
	{
		registerSerialization<ParticleHitHardFactory>();
	}

	std::shared_ptr<AW::Particle> ParticleHitHardFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);
		std::uniform_int_distribution<std::mt19937::result_type> randomClip(0, 64);

		const auto particle = std::make_shared<ParticleHitHard>(modules->texture);
		particle->x = AW::NumberHelper::random(-bundle.rect.w, bundle.rect.w);
		particle->y = AW::NumberHelper::random(-bundle.rect.h, bundle.rect.h);
		particle->ogW = AW::NumberHelper::random(2, 6);
		particle->ogH = AW::NumberHelper::random(2, 6);
		particle->r = randomRotation(rng);
		particle->ogXV = AW::NumberHelper::random(-25, 25);
		particle->ogYV = AW::NumberHelper::random(-25, 25);
		particle->totalLife = 250.0;
		particle->timeLeft = 250.0;
		particle->clip = AW::Rect(
			AW::NumberHelper::random(0, 256),
			AW::NumberHelper::random(0, 256),
			particle->ogW,
			particle->ogH
		);

		return particle;
	}
}

