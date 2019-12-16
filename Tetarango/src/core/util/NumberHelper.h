#pragma once

#include <random>
#include <math.h>
#include <algorithm>

namespace AWCore
{
	class NumberHelper
	{
		static bool hasInit;
		static std::mt19937 rng;

	public:

		static const double PI;
		static const double degToRad;
		static const double radToDeg;
		static void seedRng(const std::string& seed);
		static void resetSeed();
		static std::mt19937* getRng();

		static inline bool doubleZero(double d)
		{
			return std::abs(d) < 0.01;
		}

		static inline bool doubleZero(double a, double b)
		{
			return doubleZero(a) && doubleZero(b);
		}

		static inline bool doubleAbsGT(double a, double b)
		{
			return std::abs(a) > std::abs(b);
		}

		static inline int randomInt(int min, int max)
		{
			int range = std::abs(max - min);
			std::uniform_int_distribution<std::mt19937::result_type> random(0, range);

			return random(*getRng()) + min;
		}

		static inline double random()
		{
			std::uniform_int_distribution<std::mt19937::result_type> random(0, 1000000);
			return random(*getRng()) / 1000000.0;
		}

		static inline double random(double min, double max)
		{
			return (random() * (max - min)) + min;
		}

		static inline double random(double max)
		{
			return random(0, max);
		}

		static inline bool chance(unsigned int num, unsigned int denom = 100)
		{
			return num != 0 && randomInt(0, denom) < (int)num;
		}

		template <typename T>
		static inline T clamp(const T value, const T min, const T max)
		{
			return std::min(max, std::max(value, min));
		}

		static inline double clampWrap(const double value, const double start, const double end)
		{
			const double width = end - start;
			const double offsetValue = value - start;

			return (offsetValue - (std::floor(offsetValue / width) * width)) + start;
		}

		template <typename T>
		static inline T lerp(const T a, const T b, double p)
		{
			return (T)(((double)a * (1 - p)) + ((double)b * p));
		}
	};
}
