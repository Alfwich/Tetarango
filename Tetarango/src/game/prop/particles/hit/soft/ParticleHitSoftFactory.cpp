#include "ParticleHitSoftFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitSoft.h"

namespace AWGame
{
	ParticleHitSoftFactory::ParticleHitSoftFactory()
	{
		enableSerialization<ParticleHitSoftFactory>();
	}

	std::shared_ptr<AWCore::Particle> ParticleHitSoftFactory::makeParticle(AWCore::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);
		std::uniform_int_distribution<std::mt19937::result_type> randomClip(0, 64);

		const auto particle = std::make_shared<ParticleHitSoft>(modules->texture);
		particle->x = AWCore::NumberHelper::random(-bundle.rect.w, bundle.rect.w);
		particle->y = AWCore::NumberHelper::random(-bundle.rect.h, bundle.rect.h);
		particle->ogW = AWCore::NumberHelper::random(6, 12);
		particle->ogH = AWCore::NumberHelper::random(6, 12);
		particle->r = randomRotation(rng);
		particle->ogXV = AWCore::NumberHelper::random(-100, 100);
		particle->ogYV = AWCore::NumberHelper::random(-100, 100);
		particle->totalLife = 350.0;
		particle->timeLeft = 350.0;
		particle->clip = AWCore::Rect(
			AWCore::NumberHelper::random(0, 256),
			AWCore::NumberHelper::random(0, 256),
			particle->ogW,
			particle->ogH
		);



		return particle;
	}
}

