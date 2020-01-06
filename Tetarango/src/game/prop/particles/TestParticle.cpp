#include "TestParticle.h"

namespace AWGame
{

	TestParticle::TestParticle(std::shared_ptr<AW::TextureContainer> textureContainer)
	{
		particleTexture = textureContainer->getTexture("test-particle");
	}

	void TestParticle::onEnterFrame(const double& deltaTime)
	{
		AW::Particle::onEnterFrame(deltaTime);
	}

}
