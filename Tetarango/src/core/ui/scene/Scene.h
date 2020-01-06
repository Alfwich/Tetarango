#pragma once

#include "engine/GameObject.h"
#include "engine/module/thread/INotifyOnCompletion.h"

namespace AW
{
	class SceneContainer;

	class Scene : public GameObject, public Renderable
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
