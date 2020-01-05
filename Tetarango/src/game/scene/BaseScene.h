#pragma once

#include "ui/scene/Scene.h"
#include "GameStoragePaths.h"
#include "gui/IGuiListener.h"
#include "Scenes.h"

namespace AWGame
{

	class BaseScene : public AW::Scene, public IGuiListener
	{
	protected:
		static const Scenes& getScenes();

	public:

		static std::string sceneToStr(SceneGame scene);

		static std::string storagePath(StorePaths path);
		static std::string filePath(StorePaths path);

		BaseScene() {};
		BaseScene(SceneGame scene);

		int getDisplayScreenWidth();
		int getDisplayScreenHeight();

		virtual void transitionToScene(SceneGame scene);
		virtual void transitionToScene(std::string sceneName);

		virtual void transitionToSceneWithBundle(SceneGame scene, AW::SceneTransitionBundle& bundle);
		virtual void transitionToSceneWithBundle(std::string sceneName, AW::SceneTransitionBundle& bundle);

		std::weak_ptr<GameObject> baseSceneWeakThisRef();

		virtual void onButtonClicked(int id) { /* NO-OP */ };
		virtual void onAboutToSave() {};
	};
}
