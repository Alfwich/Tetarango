#pragma once

#include <string>
#include <map>

namespace MTGame
{

	enum class PropParticleFactory 
	{
		ParticleFactoryTest
	};

	enum class PropBlock
	{
		BasicBlock
	};

	class Props
	{
	public:

		const std::map<PropParticleFactory, std::string> particleFactories{
			{ PropParticleFactory::ParticleFactoryTest, "prop-test-particle-factory" },
		};

		const std::map<PropBlock, std::string> blocks{
			{ PropBlock::BasicBlock, "prop-basic-block" }
		};
	};

}
