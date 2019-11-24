#pragma once

#include <memory>
#include "engine/module/SystemModuleBundle.h"
#include "Props.h"

namespace MTGame
{
	class PropInfo
	{
	public:
		std::string displayName;
		double weight;
	};

	class BaseProp
	{
		static bool isLoaded;

		enum class PropType
		{
			Unspecified,
			ParticleFactory,
			Block
		};

	protected:
		static const Props& getProps();

		const PropType propType;
		const std::string propBaseName;

	public:
		static void primeProps();
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> modules);

		PropInfo info;

		BaseProp();
		BaseProp(PropParticleFactory factory);
		BaseProp(PropBlock block);
	};

}
