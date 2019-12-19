#include "ParticleSpaceBackground.h"


namespace AWGame
{
	void ParticleSpaceBackground::onEnterFrame(double deltaTime)
	{
		if (alphaMod < 255.0)
		{
			alphaMod += deltaTime * 255.0;
		}
		w = ogW * timeLeft / totalLife;
		h = ogH * timeLeft / totalLife;
		x += xV * deltaTime;
		y += yV * deltaTime;
	}

}
