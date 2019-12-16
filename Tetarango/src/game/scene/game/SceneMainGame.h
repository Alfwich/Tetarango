#pragma once

#include <memory>

#include "ui/scene/Scene.h"
#include "ui/renderable/element/Text.h"
#include "scene/BaseScene.h"
#include "engine/module/thread/INotifyOnCompletion.h"
#include "ui/renderable/primitive/particle/ParticleSystem.h"
#include "gui/menu/GameMainMenu.h"
#include "gui/transition/TransitionFade.h"
#include "gui/transition/INotifyOnFade.h"

namespace AWGame
{

	class BaseHud;

	class SceneMainGame : public BaseScene, public INotifyOnFade
	{
		std::shared_ptr<TransitionFade> globalTransition;
		std::shared_ptr<AWCore::ParticleSystem> globalParticleSystem;
		std::shared_ptr<AWCore::SceneContainer> masterSceneContainer;
		std::shared_ptr<AWCore::Text> gameLabel;
		std::shared_ptr<GameMainMenu> gameMainMenu;

		std::shared_ptr<BaseHud> hud;

		AWCore::SceneTransitionBundle transitionBundle;
		std::string getSaveSlotFilePath();
		std::string storageScopeName, nextSceneName;
		bool isSavingData = false, hasSavedData = false, hasFinishedTransition = false, backOutToMainMenuAfterSave = false, menuEnabled = false;

	public:
		SceneMainGame();

		void onInitialAttach();
		void onCreateChildren();
		void onDisplayProvisioned();
		void onChildrenHydrated();
		void onKeyPressed(SDL_Scancode key);
		void onWorkError(AWCore::WORKER_ID workerId, WorkerTaskCode code);
		void onWorkDone(AWCore::WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<AWCore::AsyncResultBundle> result);
		void onFadeOut();
		void onFadeIn();

		bool isTransitioning();
		void transitionToSceneWithBundle(std::string scene, AWCore::SceneTransitionBundle& bundle);
		void transitionToScene(SceneGame scene);
		void transitionToScene(std::string scene);
		void backOutToMainMenu();

		void saveGameData();
		void enableMenu();
		void disableMenu();
		void showOptions();
		void hideOptions();

		std::shared_ptr<AWCore::SerializationClient> doSerialize(AWCore::SerializationHint hint);
	};

}

#include "gui/hud/BaseHud.h"