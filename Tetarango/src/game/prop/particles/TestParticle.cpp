#include "TestParticle.h"


namespace AWGame
{

	TestParticle::TestParticle(std::shared_ptr<MT::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("test-particle");
	}

	void TestParticle::onEnterFrame(double deltaTime)
	{
		MT::Particle::onEnterFrame(deltaTime);
	}

}
