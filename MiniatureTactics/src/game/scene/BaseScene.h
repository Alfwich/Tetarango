#pragma once

#include "ui/scene/Scene.h"
#include "GameStoragePaths.h"
#include "gui/button/IButtonListener.h"
#include "Scenes.h"

namespace MTGame
{

	class BaseScene : public MT::Scene, public IButtonListener
	{
		static bool isLoaded;

	protected:
		static const Scenes& getScenes();

	public:

		static void primeScenes();
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> modules);

		static std::string sceneToStr(SceneGame scene);

		static std::string storagePath(StorePaths path);
		static std::string filePath(StorePaths path);

		BaseScene() {};
		BaseScene(SceneGame scene);

		int getScreenWidth();
		int getScreenHeight();

		virtual void transitionToScene(SceneGame scene);
		virtual void transitionToScene(std::string sceneName);

		virtual void transitionToSceneWithBundle(SceneGame scene, MT::SceneTransitionBundle& bundle);
		virtual void transitionToSceneWithBundle(std::string sceneName, MT::SceneTransitionBundle& bundle);

		std::weak_ptr<ApplicationObject> baseSceneWeakThisRef();

		virtual void onButtonClicked(int id) { /* NO-OP */ };
		virtual void onAboutToSave() {};
	};
}
