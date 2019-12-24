#include "ParticleFiringFactory.h"

#include "util/NumberHelper.h"
#include <random>
#include "ParticleFiring.h"

namespace AWGame
{
	ParticleFiringFactory::ParticleFiringFactory()
	{
		registerGameObject<ParticleFiringFactory>();
	}

	std::shared_ptr<AW::Particle> ParticleFiringFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleFiring>(modules->texture);
		particle->zIndex = 1;
		particle->ogW = AW::NumberHelper::random(10, 20);
		particle->ogH = AW::NumberHelper::random(10, 20);
		particle->x = -particle->ogW / 2.0;
		particle->y = -particle->ogH / 2.0;
		particle->r = AW::NumberHelper::random(0, 359);
		particle->totalLife = 100.0;
		particle->timeLeft = 100.0;
		particle->clip = AW::Rect(
			AW::NumberHelper::random(0, 256),
			AW::NumberHelper::random(0, 128),
			4, 4
		);

		return particle;
	}
}

