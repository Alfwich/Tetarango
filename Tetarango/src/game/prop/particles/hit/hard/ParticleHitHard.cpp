#include "ParticleHitHard.h"

namespace AWGame
{

	ParticleHitHard::ParticleHitHard(std::shared_ptr<AW::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("particle-hit-hard");
	}

	void ParticleHitHard::onEnterFrame(double frameTime)
	{
		x += xV * frameTime;
		y += yV * frameTime;

		double position = timeLeft / totalLife;

		w = ogW * position;
		h = ogH * position;
	}

}
