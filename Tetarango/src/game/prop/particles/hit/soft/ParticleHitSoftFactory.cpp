#include "ParticleHitSoftFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleHitSoft.h"

namespace AWGame
{
	ParticleHitSoftFactory::ParticleHitSoftFactory()
	{
		registerGameObject<ParticleHitSoftFactory>();
	}

	std::shared_ptr<AW::Particle> ParticleHitSoftFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);
		std::uniform_int_distribution<std::mt19937::result_type> randomClip(0, 64);

		const auto particle = std::make_shared<ParticleHitSoft>(modules->texture);
		particle->x = AW::NumberHelper::random(-bundle.rect.w, bundle.rect.w);
		particle->y = AW::NumberHelper::random(-bundle.rect.h, bundle.rect.h);
		particle->ogW = AW::NumberHelper::random(6, 12);
		particle->ogH = AW::NumberHelper::random(6, 12);
		particle->r = randomRotation(rng);
		particle->ogXV = AW::NumberHelper::random(-100, 100);
		particle->ogYV = AW::NumberHelper::random(-100, 100);
		particle->totalLife = 350.0;
		particle->timeLeft = 350.0;
		particle->clip = AW::Rect(
			AW::NumberHelper::random(0, 256),
			AW::NumberHelper::random(0, 256),
			particle->ogW,
			particle->ogH
		);



		return particle;
	}
}

