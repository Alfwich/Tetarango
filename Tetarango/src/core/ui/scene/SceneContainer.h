#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "engine/GameObject.h"
#include "Scene.h"
#include "SceneTransitionBundle.h"

namespace AW
{

	class SceneContainer : public GameObject, public Renderable
	{
		std::unordered_map<std::string, std::shared_ptr<Scene>> sceneMap;
		std::shared_ptr<Scene> currentScene;
		std::string currentSceneName;

	public:
		SceneContainer();
		SceneContainer(std::vector<std::shared_ptr<Scene>> scenes);

		std::weak_ptr<GameObject> notifyOnTransition;

		void add(std::shared_ptr<GameObject> ao);
		void remove(std::shared_ptr<GameObject> ao);

		bool transitionToScene(std::string name);
		bool transitionToSceneWithBundle(std::string sceneName, SceneTransitionBundle& bundle);

		std::string getCurrentSceneName();
		std::shared_ptr<Scene> getSceneForName(std::string name);
		std::shared_ptr<Scene> getCurrentScene();
		void disableCurrentScene();
		std::vector<std::string> getAvailableScenes();

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual std::vector<std::shared_ptr<ISerializable>> getSerializableChildren();
		virtual void childHydrated(std::shared_ptr<ISerializable> child);

		void onAttach();
		void onDisplayProvisioned();
	};

}
