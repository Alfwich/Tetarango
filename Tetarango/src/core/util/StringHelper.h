#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <ctype.h>

namespace AW
{

	class StringHelper
	{
	public:
		bool static inline startsWith(std::string* source, std::string val)
		{
			if (source->size() < val.size())
			{
				return false;
			}

			for (unsigned int i = 0; i < val.size(); ++i)
			{
				if ((*source)[i] != val[i])
				{
					return false;
				}
			}

			return true;
		}

		bool static inline startsWith_Offset(std::string* source, int sourceOffset, std::string val)
		{
			if (source->size() < val.size())
			{
				return false;
			}

			for (unsigned int i = 0; i < val.size(); ++i)
			{
				if (i + sourceOffset >= source->size() || (*source)[i + sourceOffset] != val[i])
				{
					return false;
				}
			}

			return true;
		}

		bool static inline startsWith(std::string* source, std::string* val)
		{
			if ((*source).size() < (*val).size())
			{
				return false;
			}

			for (unsigned int i = 0; i < (*val).size(); ++i)
			{
				if ((*source)[i] != (*val)[i])
				{
					return false;
				}
			}

			return true;
		}

		int static inline distanceToLeft(std::string* source, std::string val)
		{
			return StringHelper::distanceToLeft_offset(source, 0, val);
		}

		int static inline distanceToLeft_offset(std::string* source, int sourceOffset, std::string val)
		{
			for (unsigned int i = sourceOffset; i < source->size(); ++i)
			{
				if ((*source)[i] == val[0] && StringHelper::startsWith_Offset(source, i, val))
				{
					return i - sourceOffset;
				}
			}

			return -1;
		}

		std::string static inline getSliceOfCharVector(std::string* source, unsigned int start, unsigned int length)
		{
			if (start + length > source->size())
			{
				length = (unsigned int)source->size() - start;
			}

			std::string result;

			for (unsigned int i = start; i < start + length; ++i)
			{
				result += (*source)[i];
			}

			return result;
		}

		bool static inline ends_with(std::string const & value, std::string const & ending)
		{
			if (ending.size() > value.size()) return false;
			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

		std::string static inline convertAndPad(std::string str, const size_t num, const char paddingChar)
		{
			if (num > str.size())
				str.insert(0, num - str.size(), paddingChar);
			return str;
		}

		std::string static inline padLeft(std::string str, const size_t num, const char paddingChar)
		{
			while (str.size() < num)
			{
				str = paddingChar + str;
			}

			return str;
		}

		std::string static inline repeat(const char c, const size_t num)
		{
			auto str = std::string();
			while (str.size() < num)
			{
				str = str + c;
			}

			return str;
		}


		std::string static inline keyToDisplayName(std::string const& value)
		{
			auto result = std::string();

			bool upperNext = true;
			for (const auto c : value)
			{
				if (c == '_' || c == '-')
				{
					result += ' ';
					upperNext = true;
					continue;
				}

				if (upperNext)
				{
					result += toupper(c);
					upperNext = false;
				}
				else
				{
					result += c;
				}
			}

			return result;
		}

		int static inline getDisplayComponentForDisplayString(std::string* inStr, int cmp)
		{
			const auto str = std::string(*inStr);
			return getDisplayComponentForDisplayString(str, cmp);
		}


		int static inline getDisplayComponentForDisplayString(const std::string& inStr, int cmp)
		{
			auto str = std::string(inStr);
			int delimitPos = distanceToLeft(&str, "x");

			if (delimitPos == -1)
			{
				return 0;
			}

			if (cmp == 0)
			{
				return std::stoi(getSliceOfCharVector(&str, 0, delimitPos));
			}
			else if (cmp == 1)
			{
				return std::stoi(getSliceOfCharVector(&str, delimitPos + 1, (unsigned int)str.size() - delimitPos));
			}

			return 0;
		}

		bool static inline contains(const std::string& a, const std::string& b)
		{
			return a.find(b) != std::string::npos;
		}

	};

}
