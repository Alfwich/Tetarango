#include "TestParticle.h"


namespace AWGame
{

	TestParticle::TestParticle(std::shared_ptr<AWCore::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("test-particle");
	}

	void TestParticle::onEnterFrame(double deltaTime)
	{
		AWCore::Particle::onEnterFrame(deltaTime);
	}

}
