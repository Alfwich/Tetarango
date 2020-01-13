#pragma once

#include "engine/module/SystemModuleBundle.h"
#include "Props.h"

namespace AWGame
{
	class PropInfo
	{
	public:
		std::string displayName;
		double weight = 0.0;
	};

	class BaseProp
	{
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
		PropInfo info;

		BaseProp();
		BaseProp(PropParticleFactory factory);
		BaseProp(PropBlock block);
	};

}
