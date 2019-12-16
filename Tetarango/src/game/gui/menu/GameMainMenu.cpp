#include "GameMainMenu.h"

#include "ui/renderable/element/Rectangle.h"
#include "ui/scene/SceneContainer.h"
#include "scene/BaseScene.h"
#include "scene/game/SceneMainGame.h"

namespace
{
	const int mainMenuButtonOffset = 35;
}

namespace AWGame
{

	GameMainMenu::GameMainMenu() : BaseGui(GuiMenu::MenuGame)
	{
		rebuildOnLoad = true;
		enableSerialization<GameMainMenu>();
	}

	void GameMainMenu::onInitialAttach()
	{
		setTimeScope(AWCore::TimeScope::Menu);
	}

	void GameMainMenu::onCreateChildren()
	{
		const auto backgroundFade = std::make_shared<AWCore::Rectangle>();
		backgroundFade->name = "background";
		backgroundFade->setColor(0, 0, 0);
		backgroundFade->setAlpha(0.5);
		add(backgroundFade);

		const auto centeringContainer = std::make_shared<AWCore::Container>();
		centeringContainer->name = "center-c";
		centeringContainer->zIndex = 2;
		centeringContainer->setExpandToChildren(true);

		backButton = std::make_shared<ButtonBasic>();
		backButton->clickListener = weak_from_this();
		backButton->setText("Back");
		backButton->centerAlignSelf();
		centeringContainer->add(backButton);

		optionsButton = std::make_shared<ButtonBasic>();
		optionsButton->clickListener = weak_from_this();
		optionsButton->setText("Options");
		optionsButton->toBottomOf(backButton, 0, mainMenuButtonOffset);
		centeringContainer->add(optionsButton);

		saveButton = std::make_shared<ButtonBasic>();
		saveButton->clickListener = weak_from_this();
		saveButton->setText("Save");
		saveButton->toBottomOf(optionsButton, 0, mainMenuButtonOffset);
		centeringContainer->add(saveButton);

		mainMenuButton = std::make_shared<ButtonBasic>();
		mainMenuButton->clickListener = weak_from_this();
		mainMenuButton->setText("Main Menu");
		mainMenuButton->toBottomOf(saveButton, 0, mainMenuButtonOffset);
		centeringContainer->add(mainMenuButton);

		add(centeringContainer);
	}

	void GameMainMenu::onLayoutChildren()
	{
		setSize(modules->screen->getWidth(), modules->screen->getHeight());

		const auto backgroundFade = findChildWithName<AWCore::Rectangle>("background");
		backgroundFade->matchSizeAndCenter(this);

		const auto centeringContainer = findChildWithName<AWCore::Container>("center-c");
		centeringContainer->centerWithin(this);
	}

	void GameMainMenu::onButtonClicked(int id)
	{
		if (!this->visible)
		{
			return;
		}

		if (id == mainMenuButton->getId())
		{
			this->visible = false;

			const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
			if (mainGameScene != nullptr)
			{
				mainGameScene->backOutToMainMenu();
			}
			else
			{
				findFirstInParentChain<AWCore::SceneContainer>()->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
			}
		}
		else if (id == backButton->getId())
		{
			this->visible = false;
			modules->time->changeTimeFactorForScope(AWCore::TimeScope::Game, 1.0);
		}
		else if (id == saveButton->getId())
		{
			const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
			if (mainGameScene != nullptr)
			{
				mainGameScene->saveGameData();
			}
		}
		else if (id == optionsButton->getId())
		{
			const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
			if (mainGameScene != nullptr)
			{
				mainGameScene->showOptions();
			}
		}
	}

	void GameMainMenu::show()
	{
		const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
		if (mainGameScene != nullptr && mainGameScene->isTransitioning())
		{
			return;
		}

		modules->time->changeTimeFactorForScope(AWCore::TimeScope::Game, 0.0);
		visible = true;
	}

	void GameMainMenu::hide()
	{
		const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
		if (mainGameScene != nullptr && mainGameScene->isTransitioning())
		{
			return;
		}

		modules->time->changeTimeFactorForScope(AWCore::TimeScope::Game, 1.0);
		visible = false;
	}

}
