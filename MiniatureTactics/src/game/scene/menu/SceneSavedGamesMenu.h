#pragma once

#include "gui/button/ButtonBasic.h"
#include "scene/BaseScene.h"

namespace MTGame
{

	class SceneSavedGamesMenu : public BaseScene
	{
		std::shared_ptr<MT::Text> savedGamesMenuTitle;
		std::shared_ptr<ButtonBasic> saveSlot1;
		std::shared_ptr<ButtonBasic> saveSlot2;
		std::shared_ptr<ButtonBasic> saveSlot3;
		std::shared_ptr<ButtonBasic> backButton;

		std::shared_ptr<ButtonBasic> testSceneButton;
		std::shared_ptr<ButtonBasic> testSpaceSceneButton;

		void disableButtons();
		void enableButtons();

		std::string dataScopeToLoad;

	public:
		SceneSavedGamesMenu();

		void onCreateChildren();

		void onButtonClicked(int id);

		void onWorkError(MT::WORKER_ID workerId, WorkerTaskCode code);
		void onWorkDone(MT::WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<MT::AsyncResultBundle> result);
	};

}

