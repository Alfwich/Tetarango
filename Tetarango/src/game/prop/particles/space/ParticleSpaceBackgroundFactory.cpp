#include "ParticleSpaceBackgroundFactory.h"

#include "util/NumberHelper.h"

namespace AWGame
{
	ParticleSpaceBackgroundParticleFactory::ParticleSpaceBackgroundParticleFactory()
	{
		GORegister(ParticleSpaceBackgroundParticleFactory);
	}

	std::shared_ptr<AW::Particle> ParticleSpaceBackgroundParticleFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<ParticleSpaceBackground>();
		particle->zIndex = -1;
		const auto r = AW::NumberHelper::random(0, AW::NumberHelper::PI * 2.0);
		const auto rV = AW::NumberHelper::random(10, 45);
		particle->alphaMod = 0;
		particle->xV = std::cos(r) * rV;
		particle->yV = std::sin(r) * rV;
		particle->x = AW::NumberHelper::random(bundle.rect.w);
		particle->y = AW::NumberHelper::random(bundle.rect.h);
		particle->ogW = AW::NumberHelper::random(1, 4);
		particle->ogH = AW::NumberHelper::random(1, 4);
		particle->timeLeft = 20000.0;
		particle->totalLife = 20000.0;

		return particle;
	}
}

