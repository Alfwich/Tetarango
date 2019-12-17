#include "Particle.h"


namespace AW
{
	std::shared_ptr<Texture> Particle::getTexture()
	{
		return particleTexture;
	}

	void Particle::onEnterFrame(double frameTime)
	{
		x += xV * frameTime;
		y += yV * frameTime;

		w = ogW * timeLeft / totalLife;
		h = ogH * timeLeft / totalLife;
	}

}
