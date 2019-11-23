#include "GeneratorBlock.h"
#include "util/RandomGenerator.h"

namespace
{
	MT::RandomGenerator<double> defaultPawnStatGenerator;
}

namespace MTGame
{
	bool GeneratorBlock::hasInit = false;

	void GeneratorBlock::initGenerators()
	{
		hasInit = true;
	}

	GeneratorBlock::GeneratorBlock()
	{
		if (!hasInit)
		{
			initGenerators();
		}
	}
}
