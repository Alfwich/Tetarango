#include "SceneSplash.h"
#include <iostream>

namespace
{
	const char* splashTextId = "splash_text";
}

namespace MTGame
{

	SceneSplash::SceneSplash() : BaseScene(SceneGame::Splash)
	{
		enableSerialization<SceneSplash>();
	}

	void SceneSplash::onCreateChildren()
	{
		splashText = std::make_shared<MT::Text>();
		splashText->name = splashTextId;
		splashText->setFont("medium", 54);
		splashText->setText("SPLASH");
		splashText->setPosition(getScreenWidth() / 2.0, getScreenHeight() / 2.0);
		add(splashText);
	}

	void SceneSplash::onChildrenHydrated()
	{
		splashText = findChildWithName<MT::Text>(splashTextId);
	}

	void SceneSplash::onInitialAttach()
	{
		Scene::onInitialAttach();
		enableEnterFrame();
		splashTransition = modules->animation->createTransitionForTimeScope(MT::TimeScope::Menu);
		transitionTimer = modules->time->createTimer(MT::TimeScope::Menu);

		modules->input->mouse->registerMouseButton(MT::MouseButton::Left, baseSceneWeakThisRef());
		modules->input->keyboard->registerKey(SDL_SCANCODE_SPACE, baseSceneWeakThisRef());
	}

	void SceneSplash::onAttach()
	{
		state = 0;
		splashText->setAlpha(0.0);
		splashTransition->startTransition(splashText, 2000.0, splashText->getRect(), 1.0);
		transitionTimer->start();
	}

	void SceneSplash::onEnterFrame(double frameTime)
	{
		if (transitionTimer->getTicks() > 3000.0 && state == 0)
		{
			splashTransition->startTransition(splashText, 2000.0, splashText->getRect(), 0.0);
			state = 1;
		}

		if (transitionTimer->getTicks() > 5000.0 && state == 1 || state == 2)
		{
			splashTransition->finish();
			transitionToScene(SceneGame::MainMenu);
			state = 3;
		}
	}

	void SceneSplash::onMouseButtonLeftDown()
	{
		state = 2;
	}

	void SceneSplash::onKeyPressed(SDL_Scancode key)
	{
		state = 2;
	}

}
