#include "ParticleFiring.h"

namespace MTGame
{

	ParticleFiring::ParticleFiring(std::shared_ptr<MT::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("particle-firing");
	}

	void ParticleFiring::onEnterFrame(double frameTime)
	{
		double position = timeLeft / totalLife;
		w = ogW * position;
		h = ogH * position;
	}

}
