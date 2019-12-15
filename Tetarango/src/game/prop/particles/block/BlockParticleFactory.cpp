#include "BlockParticleFactory.h"

#include "util/NumberHelper.h"

namespace AWGame
{
	BlockParticle::BlockParticle(std::shared_ptr<MT::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("__solid__");
	}

	void BlockParticle::onEnterFrame(double deltaTime)
	{
		w = ogW * timeLeft / totalLife;
		h = ogH * timeLeft / totalLife;
		x += xV * deltaTime;
		y += yV * deltaTime;

		xV -= xV * 0.8 * deltaTime;
		yV += 600.0 * deltaTime;
	}

	BlockParticleFactory::BlockParticleFactory()
	{
		enableSerialization<BlockParticleFactory>();
	}

	void BlockParticleFactory::setModColor(const MT::Color* color)
	{
		if (color != nullptr)
		{
			modColor.r = color->r;
			modColor.g = color->g;
			modColor.b = color->b;
			modColor.a = color->a;
		}
	}

	std::shared_ptr<MT::Particle> BlockParticleFactory::makeParticle(MT::ParticleCreationBundle& bundle)
	{
		const auto particle = std::make_shared<BlockParticle>(modules->texture);
		particle->zIndex = -1;
		particle->cModR = modColor.r;
		particle->cModG = modColor.g;
		particle->cModB = modColor.b;
		auto pi = MT::NumberHelper::PI;
		auto r = (MT::NumberHelper::chance(50) ? pi : 0) + (MT::NumberHelper::random(-(pi / 4), pi / 4));
		const auto rV = MT::NumberHelper::random(200, 4000);
		particle->x = -(bundle.rect.w / 2.0) + MT::NumberHelper::random(bundle.rect.w);
		particle->y = -(bundle.rect.h / 2.0) + MT::NumberHelper::random(bundle.rect.h);
		particle->xV = std::cos(r) * rV;
		particle->yV = std::sin(r) * rV;
		particle->ogW = MT::NumberHelper::random(3, 12);
		particle->ogH = MT::NumberHelper::random(3, 12);
		particle->timeLeft = 10000.0;
		particle->totalLife = 10000.0;

		return particle;
	}
}