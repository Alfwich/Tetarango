#include "ParticleHitHardFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitHard.h"

namespace AWGame
{
	ParticleHitHardFactory::ParticleHitHardFactory()
	{
		enableSerialization<ParticleHitHardFactory>();
	}

	std::shared_ptr<AWCore::Particle> ParticleHitHardFactory::makeParticle(AWCore::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);
		std::uniform_int_distribution<std::mt19937::result_type> randomClip(0, 64);

		const auto particle = std::make_shared<ParticleHitHard>(modules->texture);
		particle->x = AWCore::NumberHelper::random(-bundle.rect.w, bundle.rect.w);
		particle->y = AWCore::NumberHelper::random(-bundle.rect.h, bundle.rect.h);
		particle->ogW = AWCore::NumberHelper::random(2, 6);
		particle->ogH = AWCore::NumberHelper::random(2, 6);
		particle->r = randomRotation(rng);
		particle->ogXV = AWCore::NumberHelper::random(-25, 25);
		particle->ogYV = AWCore::NumberHelper::random(-25, 25);
		particle->totalLife = 250.0;
		particle->timeLeft = 250.0;
		particle->clip = AWCore::Rect(
			AWCore::NumberHelper::random(0, 256),
			AWCore::NumberHelper::random(0, 256),
			particle->ogW,
			particle->ogH
		);

		return particle;
	}
}

