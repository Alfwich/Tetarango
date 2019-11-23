#include "ParticleHitHard.h"

namespace MTGame
{

	ParticleHitHard::ParticleHitHard(std::shared_ptr<MT::TextureContainer> textureContainer)
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
