#include "ParticleHitSoft.h"

namespace AWGame
{

	ParticleHitSoft::ParticleHitSoft(std::shared_ptr<AW::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("particle-hit-soft");
	}

	void ParticleHitSoft::onEnterFrame(const double& frameTime)
	{
		x += xV * frameTime;
		y += yV * frameTime;

		double position = timeLeft / totalLife;

		w = ogW * position;
		h = ogH * position;
	}

}
