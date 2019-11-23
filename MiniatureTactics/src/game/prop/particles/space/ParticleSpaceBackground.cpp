#include "ParticleSpaceBackground.h"


namespace MTGame
{

	ParticleSpaceBackground::ParticleSpaceBackground(std::shared_ptr<MT::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("__solid__");
	}

	void ParticleSpaceBackground::onEnterFrame(double deltaTime)
	{
	}

}
