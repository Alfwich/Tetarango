#pragma once

#include <string>
#include <unordered_map>

namespace AW
{
	class GameObject;

	class SceneTransitionBundle
	{
		std::unordered_map<std::string, std::shared_ptr<GameObject>> objects;
	public:

		std::string data;
		std::string entranceZoneName;

		void putObject(std::shared_ptr<GameObject> ao)
		{
			if (ao == nullptr)
			{
				return;
			}

			objects[ao->name] = ao;
			ao->removeFromParent();
		}

		void putObject(std::string name, std::shared_ptr<GameObject> ao)
		{
			if (ao == nullptr && objects.count(name) == 0)
			{
				return;
			}

			objects[name] = ao;
			ao->removeFromParent();
		}

		void putObjects(std::string name, std::vector<std::shared_ptr<GameObject>> aos)
		{
			for (const auto ao : aos)
			{
				putObject(ao);
			}
		}


		template <typename T>
		std::shared_ptr<T> getObject(std::string name)
		{
			if (objects.count(name) == 0)
			{
				return nullptr;
			}

			const auto obj = objects.at(name);
			const auto result = std::dynamic_pointer_cast<T>(obj);

			if (result != nullptr)
			{
				objects.erase(name);
			}

			return result;
		}

	};


}

#include "engine/GameObject.h"
