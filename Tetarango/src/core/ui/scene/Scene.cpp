#include "Scene.h"

namespace AW
{
	Scene::Scene() : Scene(std::string())
	{
	}

	Scene::Scene(std::string sceneName)
	{
		this->sceneName = sceneName;
		activate();
		GORegister(Scene);
	}

	std::shared_ptr<SceneContainer> Scene::getSceneContainer()
	{
		return findFirstInParentChain<SceneContainer>();
	}

	std::string Scene::getSceneName()
	{
		return sceneName;
	}

	void Scene::setSceneName(std::string name)
	{
		sceneName = name;
	}

	std::shared_ptr<SerializationClient> Scene::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__scene__", hint);
		sceneName = client->serializeString("sn", sceneName);

		Renderable::doManualSerialize(hint, client);

		return GameObject::doSerialize(hint);
	}
}
