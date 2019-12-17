#include "ParticleFiring.h"

namespace AWGame
{

	ParticleFiring::ParticleFiring(std::shared_ptr<AW::TextureContainer> textureContainer)
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
