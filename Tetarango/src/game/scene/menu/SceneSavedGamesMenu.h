#pragma once

#include "gui/button/ButtonBasic.h"
#include "scene/BaseScene.h"

namespace AWGame
{

	class SceneSavedGamesMenu : public BaseScene
	{
		std::shared_ptr<AW::Text> savedGamesMenuTitle;

		std::shared_ptr<ButtonBasic> saveSlot1, saveSlot2, saveSlot3, backButton;
		std::shared_ptr<ButtonBasic> testScene1, testScene2, testScene3;

		void disableButtons();
		void enableButtons();

		std::string dataScopeToLoad;

	public:
		SceneSavedGamesMenu();

		void onCreateChildren();

		void onButtonClicked(int id);

		void onWorkError(AW::WORKER_ID workerId, AW::WorkerTaskCode code);
		void onWorkDone(AW::WORKER_ID workerId, AW::WorkerTaskCode code, std::shared_ptr<AW::AsyncResultBundle> result);
	};

}

