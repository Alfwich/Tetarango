#pragma once

namespace AW
{

	template<typename T>
	class RandomGenerator
	{
		std::map<T, double> probabilityOfOutcome;
		std::map<T, std::shared_ptr<RandomGenerator>> subGenerators;
		std::vector<std::pair<unsigned int, T>> packedProbabilities;

		bool hasPackedProbabilities = false;
		const int maxRngInt = INT_MAX / 64; // Expand if we notice issues - or deal with super rare items

		void packProbabilities();

	public:

		void registerCase(T outcome, double probability = 1.0);
		void registerSubGenerator(T outcome, std::shared_ptr<RandomGenerator> generator);
		T get(double probOffset = 0.0);
	};

	template<typename T>
	inline void RandomGenerator<T>::packProbabilities()
	{
		hasPackedProbabilities = true;
		packedProbabilities.clear();

		std::vector<std::pair<T, double>> probs;
		auto universeProb = 0.0;

		for (auto it = probabilityOfOutcome.begin(); it != probabilityOfOutcome.end(); ++it)
		{
			universeProb += (*it).second;
			probs.push_back((*it));
		}

		for (auto it = probs.begin(); it != probs.end(); ++it)
		{
			(*it).second /= universeProb;
		}

		std::sort(probs.begin(), probs.end(), [](const std::pair<T, double>& a, const std::pair<T, double>& b) { return a.second < b.second; });

		unsigned int probOffset = 0;
		for (auto it = probs.begin(); it != probs.end(); ++it)
		{
			const auto outcome = (*it).first;
			const auto probability = (*it).second;

			probOffset += (unsigned int)(maxRngInt * probability);
			packedProbabilities.push_back(std::make_pair((unsigned int)probOffset, outcome));
		}
	}

	template<typename T>
	inline void RandomGenerator<T>::registerCase(T outcome, double probability)
	{
		hasPackedProbabilities = false;
		probabilityOfOutcome[outcome] = probability;
	}

	template<typename T>
	inline void RandomGenerator<T>::registerSubGenerator(T outcome, std::shared_ptr<RandomGenerator> generator)
	{
		subGenerators[outcome] = generator;
	}

	template<typename T>
	inline T RandomGenerator<T>::get(double probOffset)
	{
		if (probabilityOfOutcome.empty())
		{
			return T();
		}

		if (!hasPackedProbabilities)
		{
			packProbabilities();
		}

		const unsigned int rng = NumberHelper::randomInt(0, maxRngInt);
		const unsigned int offset = (unsigned int)(maxRngInt * probOffset);

		for (auto it = packedProbabilities.begin(); it != packedProbabilities.end(); ++it)
		{
			if (rng <= (*it).first + offset)
			{
				const auto result = (*it).second;
				if (subGenerators.count(result))
				{
					return subGenerators.at(result)->get();
				}

				return (*it).second;
			}
		}

		return packedProbabilities.back().second;
	}

}
