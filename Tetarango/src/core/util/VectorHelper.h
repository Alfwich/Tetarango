#pragma once

#include <string>
#include <vector>

#include "util/StringHelper.h"

namespace AW
{
	class VectorHelper
	{
	public:
		static inline std::string intVectorToString(std::vector<int>& input)
		{
			auto result = std::string();

			for (const auto i : input)
			{
				result += std::to_string(i) + ",";
			}

			return result;
		}

		static inline std::vector<int> stringToIntVector(std::string input)
		{
			auto result = std::vector<int>();
			auto pos = 0;

			while (pos < input.size())
			{
				int nextComma = StringHelper::distanceToLeft_offset(&input, pos, ",");
				if (pos >= nextComma)
				{
					break;
				}

				result.push_back(std::stoi(input.substr(pos, nextComma - pos)));
				pos = nextComma;
			}

			return result;
		}
	};
}
