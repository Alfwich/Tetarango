#include "SceneMainMenu.h"

namespace
{
	const double titleDistanceFromMenuItems = 200.0;
	const double menuItemDistance = 35.0;
}

namespace AWGame
{

	SceneMainMenu::SceneMainMenu() : BaseScene(SceneGame::MainMenu)
	{
		setShouldRebuildOnLoad();
		registerSerialization<SceneMainMenu>();
	}

	void SceneMainMenu::onInitialAttach()
	{
		BaseScene::onInitialAttach();

		gameTitleTransition = modules->animation->createTransitionForTimeScope(AW::TimeScope::Menu);
	}

	void SceneMainMenu::onCreateChildren()
	{
		rootContainer = std::make_shared<AW::Container>();
		rootContainer->setAlpha(0.0);
		rootContainer->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		rootContainer->topLeftAlignSelf();
		add(rootContainer);

		const auto centerContainer = std::make_shared<AW::Container>();
		centerContainer->centerAlignWithin(rootContainer);
		rootContainer->add(centerContainer);

		gameTitle = std::make_shared<TitleGame>();
		centerContainer->add(gameTitle);

		playButton = std::make_shared<ButtonBasic>();
		playButton->setText("Play");
		playButton->clickListener = baseSceneWeakThisRef();
		playButton->toBottomOf(gameTitle, 0.0, titleDistanceFromMenuItems);
		centerContainer->add(playButton);

		optionButton = std::make_shared<ButtonBasic>();
		optionButton->setText("Options");
		optionButton->clickListener = baseSceneWeakThisRef();
		optionButton->toBottomOf(playButton, 0, menuItemDistance);
		centerContainer->add(optionButton);

		exitButton = std::make_shared<ButtonBasic>();
		exitButton->setText("Exit");
		exitButton->clickListener = baseSceneWeakThisRef();
		exitButton->toBottomOf(optionButton, 0, menuItemDistance);
		centerContainer->add(exitButton);

		centerContainer->resizeSelfToChildrenAndCenterChildren();
	}

	void SceneMainMenu::onAttach()
	{
		BaseScene::onAttach();
		rootContainer->setAlpha(0.0);
		gameTitleTransition->startTransition(rootContainer, 500.0, rootContainer->getRect(), 1.0);
	}

	void SceneMainMenu::onButtonClicked(int id)
	{
		if (id == playButton->getId())
		{
			transitionToScene(SceneGame::SavedGamesMenu);
		}

		if (id == optionButton->getId())
		{
			transitionToScene(SceneGame::OptionsMenu);
		}

		if (id == exitButton->getId())
		{
			modules->event->pushEvent(std::make_shared<AW::ApplicationEvent>(AW::Events::QuitRequested));
		}
	}
}