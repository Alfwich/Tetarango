#include "SceneContainer.h"

namespace AW
{

	SceneContainer::SceneContainer()
	{
		GORegister(SceneContainer);
	}

	SceneContainer::SceneContainer(std::vector<std::shared_ptr<Scene>> scenes)
	{
		for (const auto scene : scenes)
		{
			add(scene);
		}
	}

	void SceneContainer::add(std::shared_ptr<GameObject> ao)
	{
		std::shared_ptr<Scene> scene = std::static_pointer_cast<Scene>(ao);

		if (!scene)
		{
			Logger::instance()->logCritical("SceneContainer::Failed to add child, Scene is the only type that can be added to SceneContainer");
			return;
		}

		if (sceneMap.count(scene->getSceneName()) == 1)
		{
			Logger::instance()->logCritical("SceneContainer::Failed to add scene name=" + scene->getSceneName() + ", Scene with this name is already added to the SceneContainer");
			return;
		}

		sceneMap[scene->getSceneName()] = scene;
		Logger::instance()->log("SceneContainer::Added scene name=" + scene->getSceneName());
	}

	void SceneContainer::remove(std::shared_ptr<GameObject> ao)
	{
		std::shared_ptr<Scene> scene = std::static_pointer_cast<Scene>(ao);

		if (!scene)
		{
			Logger::instance()->logCritical("SceneContainer::Failed to remove child, Scene is the only type that can be remove from SceneContainer");
			return;
		}

		if (sceneMap.count(scene->getSceneName()) == 0)
		{
			Logger::instance()->logCritical("SceneContainer::Failed to remove scene name=" + scene->getSceneName() + ", a Scene with this name is not defined");
			return;
		}

		if (currentSceneName == scene->getSceneName())
		{
			disableCurrentScene();
		}

		sceneMap.erase(scene->getSceneName());
		Logger::instance()->log("SceneContainer::Removed scene name=" + scene->getSceneName());
	}

	void SceneContainer::removeScene(const std::string& name)
	{
		if (sceneMap.count(name) == 1)
		{
			remove(sceneMap[name]);
		}
	}

	bool SceneContainer::transitionToScene(const std::string& sceneName)
	{
		if (currentScene != nullptr && currentScene->getSceneName() == sceneName)
		{
			Logger::instance()->log("SceneContainer::Failed to transition to Scene sceneName=" + sceneName + ", This scene is already active");
			return false;
		}

		if (sceneMap.count(sceneName) == 0)
		{
			Logger::instance()->logCritical("SceneContainer::Failed to transition to Scene sceneName=" + sceneName + ", This scene does not exist");
			return false;
		}

		const auto callback = notifyOnTransition.lock();
		if (callback != nullptr)
		{
			callback->onWillTransitioned();
		}

		disableCurrentScene();

		currentScene = sceneMap[sceneName];
		currentSceneName = sceneName;
		GameObject::add(currentScene);
		currentScene->layout();
		Logger::instance()->log("SceneContainer::Transitioned to scene sceneName=" + sceneName);

		return true;
	}

	bool SceneContainer::transitionToSceneWithBundle(const std::string& sceneName, SceneTransitionBundle& bundle)
	{
		if (currentScene && currentScene->getSceneName() == sceneName)
		{
			Logger::instance()->log("SceneContainer::Failed to transition to Scene sceneName=" + sceneName + ", This scene is already active");
			return false;
		}

		if (sceneMap.count(sceneName) == 0)
		{
			Logger::instance()->logCritical("SceneContainer::Failed to transition to Scene sceneName=" + sceneName + ", This scene does not exist");
			return false;
		}

		const auto callback = notifyOnTransition.lock();
		if (callback != nullptr)
		{
			callback->onWillTransitioned();
		}

		disableCurrentScene();

		currentScene = sceneMap[sceneName];
		currentSceneName = sceneName;
		sceneMap[sceneName]->onTransitionedTo(bundle);
		GameObject::add(currentScene);
		Logger::instance()->log("SceneContainer::Transitioned to scene sceneName=" + sceneName);

		return true;
	}

	std::string SceneContainer::getCurrentSceneName()
	{
		return currentSceneName;
	}

	std::shared_ptr<Scene> SceneContainer::getSceneForName(std::string name)
	{
		if (sceneMap.count(name) == 1)
		{
			return sceneMap[name];
		}

		return nullptr;
	}

	std::shared_ptr<Scene> SceneContainer::getCurrentScene()
	{
		return currentScene;
	}

	void SceneContainer::disableCurrentScene()
	{
		if (currentScene == nullptr)
		{
			return;
		}

		GameObject::remove(currentScene);

		currentScene = nullptr;
		currentSceneName = std::string();
	}

	std::vector<std::string> SceneContainer::getAvailableScenes()
	{
		auto keys = std::vector<std::string>();

		for (const auto sceneKeyToScene : sceneMap)
		{
			keys.push_back(sceneKeyToScene.first);
		}

		return keys;
	}

	bool SceneContainer::hasScene(const std::string& name)
	{
		const auto sceneNames = getAvailableScenes();
		return std::find(sceneNames.begin(), sceneNames.end(), name) != sceneNames.end();
	}

	std::shared_ptr<SerializationClient> SceneContainer::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__scene_container__", hint);
		currentSceneName = client->serializeString("cs", currentSceneName);

		Renderable::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}

	std::vector<std::shared_ptr<ISerializable>> SceneContainer::getSerializableChildren()
	{
		std::vector<std::shared_ptr<ISerializable>> scenesToSerialize;

		for (const auto scene : sceneMap)
		{
			scenesToSerialize.push_back(scene.second);
		}

		return scenesToSerialize;
	}

	void SceneContainer::childHydrated(std::shared_ptr<ISerializable> child)
	{
		softAddedChildren.push_back(child);
	}

	void SceneContainer::onAttach()
	{
		if (currentSceneName.size() > 0 && currentScene == nullptr)
		{
			transitionToScene(currentSceneName);
		}
	}

	void SceneContainer::onHandleApplicationEvent(ApplicationEvent* event)
	{
		if (event->code == Events::UpEvent && event->message == "changescene" && event->data != nullptr)
		{
			const auto sceneName = *(std::string*)event->data;
			if (hasScene(sceneName))
			{
				transitionToScene(sceneName);
				event->stopPropagation = true;
			}
		}

		GameObject::onHandleApplicationEvent(event);
	}

	void SceneContainer::onDisplayProvisioned()
	{
		GameObject::onDisplayProvisioned();

		if (currentScene != nullptr)
		{
			currentScene->onDisplayProvisioned();

			if (currentScene->shouldRebuildOnLoad())
			{
				currentScene->rebuild();
			}
			else if (currentScene->shouldLayoutOnLoad())
			{
				currentScene->layout();
			}
		}

	}
}
