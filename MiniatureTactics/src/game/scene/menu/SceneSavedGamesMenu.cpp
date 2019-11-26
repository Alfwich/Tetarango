#include "SceneSavedGamesMenu.h"

#include "scene/game/SceneMainGame.h"

namespace
{
	const double buttonVerticalPaddingFactor = 0.1;
}

namespace MTGame
{

	SceneSavedGamesMenu::SceneSavedGamesMenu() : BaseScene(SceneGame::SavedGamesMenu)
	{
		rebuildOnLoad = true;
		enableSerialization<SceneSavedGamesMenu>();
	}

	void SceneSavedGamesMenu::onCreateChildren()
	{
		const auto rootContainer = std::make_shared<MT::Container>();
		rootContainer->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		rootContainer->centerAlignSelf();
		add(rootContainer);

		savedGamesMenuTitle = std::make_shared<MT::Text>();
		savedGamesMenuTitle->setFont("medium", 28);
		savedGamesMenuTitle->setText("Saved Games");
		savedGamesMenuTitle->toTopOf(rootContainer, 0, -savedGamesMenuTitle->getHalfHeight() - modules->screen->getHeight() * 0.25);
		rootContainer->add(savedGamesMenuTitle);

		const auto client = modules->storage->getClient();
		const auto buttonOffset = modules->screen->getHeight() * buttonVerticalPaddingFactor;
		const auto centerContainer = std::make_shared<MT::Container>();
		centerContainer->setExpandToChildren(true);
		centerContainer->centerWithin(rootContainer, 0, savedGamesMenuTitle->getBottom() / 2.0);
		rootContainer->add(centerContainer);

		saveSlot1 = std::make_shared<ButtonBasic>();
		saveSlot1->centerAlignSelf();
		if (client->readBool(storagePath(StorePaths::System_SaveSlot1)))
		{
			saveSlot1->setText("Load 1");
		}
		else
		{
			saveSlot1->setText("Empty 1");
		}
		saveSlot1->clickListener = baseSceneWeakThisRef();
		centerContainer->add(saveSlot1);

		saveSlot2 = std::make_shared<ButtonBasic>();
		saveSlot2->toBottomOf(saveSlot1, 0, buttonOffset);
		if (client->readBool(storagePath(StorePaths::System_SaveSlot2)))
		{
			saveSlot2->setText("Load 2");
		}
		else
		{
			saveSlot2->setText("Empty 2");
		}
		saveSlot2->clickListener = baseSceneWeakThisRef();
		centerContainer->add(saveSlot2);

		saveSlot3 = std::make_shared<ButtonBasic>();
		saveSlot3->toBottomOf(saveSlot2, 0, buttonOffset);
		if (client->readBool(storagePath(StorePaths::System_SaveSlot3)))
		{
			saveSlot3->setText("Load 3");
		}
		else
		{
			saveSlot3->setText("Empty 3");
		}
		saveSlot3->clickListener = baseSceneWeakThisRef();
		centerContainer->add(saveSlot3);

		backButton = std::make_shared<ButtonBasic>();
		backButton->toBottomOf(saveSlot3, 0, buttonOffset);
		backButton->setText("Back");
		backButton->clickListener = baseSceneWeakThisRef();
		centerContainer->add(backButton);

		if (modules->gameConfig->getConfigBool(Config::Param::enableTestScenes))
		{
			centerContainer->setExpandToChildren(false);
			testSceneButton = std::make_shared<ButtonBasic>();
			testSceneButton->setColor(128, 255, 255);
			testSceneButton->toRightOf(backButton, buttonOffset);
			testSceneButton->setText("Test 1");
			testSceneButton->clickListener = baseSceneWeakThisRef();
			centerContainer->add(testSceneButton);

			testSpaceSceneButton = std::make_shared<ButtonBasic>();
			testSpaceSceneButton->setColor(128, 255, 255);
			testSpaceSceneButton->toTopOf(testSceneButton, 0, buttonOffset);
			testSpaceSceneButton->setText("Test 2");
			testSpaceSceneButton->clickListener = baseSceneWeakThisRef();
			centerContainer->add(testSpaceSceneButton);
		}
	}

	void SceneSavedGamesMenu::onButtonClicked(int id)
	{
		const auto client = modules->storage->getClient();
		if (id == saveSlot1->getId())
		{
			client->writeInt(storagePath(StorePaths::System_CurrentSaveSlot), 1);
			client->writeBool(storagePath(StorePaths::System_SaveSlot1), true);
			dataScopeToLoad = filePath(StorePaths::System_SaveSlot1);
			modules->storage->loadDataScopeAsync(dataScopeToLoad, weak_from_this());
			disableButtons();
		}
		else if (id == saveSlot2->getId())
		{
			client->writeInt(storagePath(StorePaths::System_CurrentSaveSlot), 2);
			client->writeBool(storagePath(StorePaths::System_SaveSlot2), true);
			dataScopeToLoad = filePath(StorePaths::System_SaveSlot2);
			modules->storage->loadDataScopeAsync(dataScopeToLoad, weak_from_this());
			disableButtons();
		}
		else if (id == saveSlot3->getId())
		{
			client->writeInt(storagePath(StorePaths::System_CurrentSaveSlot), 3);
			client->writeBool(storagePath(StorePaths::System_SaveSlot3), true);
			dataScopeToLoad = filePath(StorePaths::System_SaveSlot3);
			modules->storage->loadDataScopeAsync(dataScopeToLoad, weak_from_this());
			disableButtons();
		}
		else if (id == backButton->getId())
		{
			transitionToScene(SceneGame::MainMenu);
		}
		else if (id == testSceneButton->getId())
		{
			transitionToScene("test_scene");
		}
		else if (id == testSpaceSceneButton->getId())
		{
			transitionToScene("test_space_scene");
		}
	}

	void SceneSavedGamesMenu::disableButtons()
	{
		saveSlot1->inputEnabled = false;
		saveSlot2->inputEnabled = false;
		saveSlot3->inputEnabled = false;
		if (testSceneButton != nullptr)
		{
			testSceneButton->inputEnabled = false;
			testSpaceSceneButton->inputEnabled = false;
		}
	}

	void SceneSavedGamesMenu::enableButtons()
	{
		saveSlot1->inputEnabled = true;
		saveSlot2->inputEnabled = true;
		saveSlot3->inputEnabled = true;
		if (testSceneButton != nullptr)
		{
			testSceneButton->inputEnabled = true;
			testSpaceSceneButton->inputEnabled = true;
		}
	}

	void SceneSavedGamesMenu::onWorkError(MT::WORKER_ID workerId, WorkerTaskCode code)
	{
		enableButtons();
	}

	void SceneSavedGamesMenu::onWorkDone(MT::WORKER_ID workerId, WorkerTaskCode code, std::shared_ptr<MT::AsyncResultBundle> result)
	{
		switch (code)
		{
		case WorkerTaskCode::STORE_LOAD_DATA:
			const auto gameData = modules->storage->getClient(dataScopeToLoad)->readSring(storagePath(StorePaths::GameData));

			// New Game
			if (gameData.size() == 0)
			{
				const auto scene = std::make_shared<SceneMainGame>();
				getSceneContainer()->add(scene);
			}
			else
			{
				const auto scene = std::dynamic_pointer_cast<SceneMainGame>(modules->serialization->hydrate(gameData));
				getSceneContainer()->add(scene);
			}

			enableButtons();

			transitionToScene(SceneGame::MainGame);

			break;
		}
	}
}