#include "TestParticle.h"


namespace MTGame
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
