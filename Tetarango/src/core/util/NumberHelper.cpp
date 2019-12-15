#include "NumberHelper.h"

namespace MT
{
	const double NumberHelper::PI = 3.14159265359;
	const double NumberHelper::degToRad = 0.01745329251;
	const double NumberHelper::radToDeg = 57.2957795131;

	bool NumberHelper::hasInit = false;
	std::mt19937 NumberHelper::rng;

	void NumberHelper::seedRng(const std::string& seed)
	{
		std::seed_seq seq(seed.begin(), seed.end());
		getRng()->seed(seq);
	}

	void NumberHelper::resetSeed()
	{
		getRng()->seed(std::random_device()());
	}

	std::mt19937* NumberHelper::getRng()
	{
		if (!hasInit)
		{
			hasInit = true;
			resetSeed();
		}

		return &rng;
	}
}