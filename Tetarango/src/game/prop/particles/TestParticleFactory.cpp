#include "TestParticleFactory.h"

#include <random>

namespace AWGame
{
	TestParticleFactory::TestParticleFactory() : TestParticleFactory(0xff, 0xff, 0xff)
	{
	}

	TestParticleFactory::TestParticleFactory(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;

		GORegister(TestParticleFactory);
	}

	std::shared_ptr<AW::Particle> TestParticleFactory::makeParticle(AW::ParticleCreationBundle& bundle)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> randomWidth(0, (int)bundle.rect.w);
		std::uniform_int_distribution<std::mt19937::result_type> randomHeight(0, (int)bundle.rect.h);
		std::uniform_int_distribution<std::mt19937::result_type> randomSize(5, 10);
		std::uniform_int_distribution<std::mt19937::result_type> randomRotation(0, 359);
		std::uniform_int_distribution<std::mt19937::result_type> xVelocity(0, (int)bundle.xVelocity);
		std::uniform_int_distribution<std::mt19937::result_type> yVelocity(0, (int)bundle.yVelocity);
		std::uniform_int_distribution<std::mt19937::result_type> randomClip(0, 64);

		const auto particle = std::make_shared<TestParticle>(modules->texture);
		particle->x = randomWidth(rng);
		particle->y = randomHeight(rng);
		particle->w = randomSize(rng);
		particle->h = randomSize(rng);
		particle->ogW = particle->w;
		particle->ogH = particle->h;
		particle->cModR = r;
		particle->cModG = g;
		particle->cModB = b;
		particle->r = randomRotation(rng);
		particle->xV = (int)(xVelocity(rng) - bundle.xVelocity / 2.0);
		particle->yV = (int)(yVelocity(rng) - bundle.yVelocity / 2.0);
		particle->totalLife = 2000.0;
		particle->timeLeft = 2000.0;
		particle->clip = AW::Rect(
			randomClip(rng),
			randomClip(rng),
			4, 4
		);

		return particle;
	}

	std::shared_ptr<AW::SerializationClient> TestParticleFactory::doSerialize(AW::SerializationHint hint)
	{
		const auto client = serializationClient->getClient("test_particle_factory", hint);

		r = client->serializeInt("r", r);
		g = client->serializeInt("g", g);
		b = client->serializeInt("b", b);

		return AW::IParticleFactory::doSerialize(hint);
	}
}

