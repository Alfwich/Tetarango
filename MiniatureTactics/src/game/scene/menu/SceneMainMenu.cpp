#include "SceneMainMenu.h"

namespace
{
	const double buttonVerticalPaddingFactor = 0.1;
}

namespace MTGame
{

	SceneMainMenu::SceneMainMenu() : BaseScene(SceneGame::MainMenu)
	{
		rebuildOnLoad = true;
		enableSerialization<SceneMainMenu>();
	}

	void SceneMainMenu::onInitialAttach()
	{
		BaseScene::onInitialAttach();

		gameTitleTransition = modules->animation->createTransitionForTimeScope(MT::TimeScope::Menu);
	}

	void SceneMainMenu::onCreateChildren()
	{
		rootContainer = std::make_shared<MT::Container>();
		rootContainer->setAlpha(0.0);
		rootContainer->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		rootContainer->centerAlignSelf();
		add(rootContainer);

		gameTitleText = std::make_shared<MT::Text>();
		gameTitleText->setFont("medium", 52);
		gameTitleText->setText(modules->gameConfig->getConfigString(Config::Param::gameName));
		gameTitleText->toTopOf(rootContainer, 0, -gameTitleText->getHalfHeight() - modules->screen->getHeight() * 0.35);
		rootContainer->add(gameTitleText);

		const auto buttonOffset = modules->screen->getHeight() * buttonVerticalPaddingFactor;
		const auto centerContainer = std::make_shared<MT::Container>();
		centerContainer->setExpandToChildren(true);
		centerContainer->centerWithin(rootContainer, 0, gameTitleText->getBottom() / 2.0);
		rootContainer->add(centerContainer);

		playButton = std::make_shared<ButtonBasic>();
		playButton->setText("Play");
		playButton->clickListener = baseSceneWeakThisRef();
		playButton->centerAlignSelf();
		centerContainer->add(playButton);

		optionButton = std::make_shared<ButtonBasic>();
		optionButton->setText("Options");
		optionButton->clickListener = baseSceneWeakThisRef();
		optionButton->toBottomOf(playButton, 0, buttonOffset);
		centerContainer->add(optionButton);

		exitButton = std::make_shared<ButtonBasic>();
		exitButton->setText("Exit");
		exitButton->clickListener = baseSceneWeakThisRef();
		exitButton->toBottomOf(optionButton, 0, buttonOffset);
		centerContainer->add(exitButton);
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
			modules->event->pushEvent(std::make_shared<MT::ApplicationEvent>(MT::Events::QuitRequested));
		}
	}
}