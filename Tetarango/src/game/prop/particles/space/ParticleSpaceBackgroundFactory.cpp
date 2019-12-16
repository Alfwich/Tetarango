#include "ParticleSpaceBackgroundFactory.h"

#include "util/NumberHelper.h"

namespace AWGame
{
	ParticleSpaceBackgroundParticleFactory::ParticleSpaceBackgroundParticleFactory()
	{
		enableSerialization<ParticleSpaceBackgroundParticleFactory>();
	}

	std::shared_ptr<AWCore::Particle> ParticleSpaceBackgroundParticleFactory::makeParticle(AWCore::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleSpaceBackground>(modules->texture);
		particle->zIndex = -1;
		const auto r = AWCore::NumberHelper::random(0, AWCore::NumberHelper::PI * 2.0);
		const auto rV = AWCore::NumberHelper::random(10, 45);
		particle->alphaMod = 0;
		particle->xV = std::cos(r) * rV;
		particle->yV = std::sin(r) * rV;
		particle->x = AWCore::NumberHelper::random(bundle.rect.w);
		particle->y = AWCore::NumberHelper::random(bundle.rect.h);
		particle->ogW = AWCore::NumberHelper::random(1, 4);
		particle->ogH = AWCore::NumberHelper::random(1, 4);
		particle->timeLeft = 20000.0;
		particle->totalLife = 20000.0;

		return particle;
	}
}

