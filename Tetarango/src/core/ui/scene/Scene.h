#pragma once

#include <string>
#include "engine/ApplicationObject.h"
#include "engine/module/thread/INotifyOnCompletion.h"

namespace MT
{
	class SceneContainer;

	class Scene : public ApplicationObject
	{
		std::string sceneName;

	protected:
		std::shared_ptr<SceneContainer> getSceneContainer();

	public:
		Scene();
		Scene(std::string sceneName);

		std::string getSceneName();
		void setSceneName(std::string name);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
	};

}

#include "SceneContainer.h"
