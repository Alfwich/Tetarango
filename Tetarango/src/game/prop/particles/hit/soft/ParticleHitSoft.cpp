#include "ParticleHitSoft.h"

namespace MTGame
{

	ParticleHitSoft::ParticleHitSoft(std::shared_ptr<MT::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("particle-hit-soft");
	}

	void ParticleHitSoft::onEnterFrame(double frameTime)
	{
		x += xV * frameTime;
		y += yV * frameTime;

		double position = timeLeft / totalLife;

		w = ogW * position;
		h = ogH * position;
	}

}
