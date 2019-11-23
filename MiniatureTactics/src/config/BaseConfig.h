#pragma once

#include <string>
#include <map>

namespace Config
{
	template <typename T>
	class BaseConfig
	{
		bool hasLoaded = false;
		std::map<T, std::string> config;

		void loadIfNeeded()
		{
			if (!hasLoaded)
			{
				config = getConfig();
				hasLoaded = true;
			}
		}

	protected:
		virtual const std::map<T, std::string> getConfig() = 0;

		const std::string TRUE = "true";
		const std::string FALSE = "false";

	public:
		std::string getConfigString(T key)
		{
			loadIfNeeded();
			if (config.count(key) == 0)
			{
				throw "BaseConfig::Key not found in config";
			}

			return config.at(key);
		}

		bool getConfigBool(T key)
		{
			return getConfigString(key) == TRUE;
		}
	};
}
