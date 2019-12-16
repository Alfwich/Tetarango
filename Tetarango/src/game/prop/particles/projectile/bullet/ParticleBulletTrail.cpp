#include "ParticleBulletTrail.h"


namespace AWGame
{

	ParticleBulletTrail::ParticleBulletTrail(std::shared_ptr<AWCore::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("__solid__");
	}

	void ParticleBulletTrail::onEnterFrame(double deltaTime)
	{
		const auto p = (timeLeft / totalLife);
		alphaMod = p * 155.0;
		h = p * ogH;
		cModR = 100.0 + p * 100.0;
		cModG = 100.0 + p * 100.0;
		cModB = 100.0 + p * 100.0;
	}

}
