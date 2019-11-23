#pragma once

#include <string>
#include <map>

namespace MTGame
{

	enum class PropParticleFactory 
	{
		ParticleFactoryTest
	};

	enum class PropBuilding
	{
		BuildingShop
	};

	class Props
	{
	public:

		const std::map<PropParticleFactory, std::string> particleFactories{
			{ PropParticleFactory::ParticleFactoryTest, "prop-test-particle-factory" },
		};

		const std::string buildingResourceName = "prop-building";
		const std::map<PropBuilding, std::string> buildings{
			{ PropBuilding::BuildingShop, "prop-building-shop-1" },
		};
	};

}
