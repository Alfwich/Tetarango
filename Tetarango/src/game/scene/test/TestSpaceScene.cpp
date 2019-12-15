#include "TestSpaceScene.h"

#include <random>
#include "GameImports.h"

namespace
{
	const auto backButtonId = "back_button";
	const auto starSystemId = "global-p-system";
	const auto player = "player";
}

namespace AWGame
{

	TestSpaceScene::TestSpaceScene() : MT::Scene("test_space_scene")
	{
		rebuildOnLoad = true;
		enableSerialization<TestSpaceScene>();
	}

	void TestSpaceScene::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(MT::TimeScope::Game);

		modules->input->keyboard->registerKeys({ SDL_SCANCODE_ESCAPE, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3 }, weak_from_this());
	}

	void TestSpaceScene::onAttach()
	{
		modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 1.0);
	}

	void TestSpaceScene::onCreateChildren()
	{
		const auto mainGameMenu = std::make_shared<GameMainMenu>();
		mainGameMenu->zIndex = 20;
		mainGameMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		mainGameMenu->renderPositionMode = MT::RenderPositionMode::Absolute;
		mainGameMenu->visible = false;
		add(mainGameMenu);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(4.0, 1.0);
		camera->setDefaultsAndReset(2.0, 0.0, 0.0);
		camera->setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(MT::TimeScope::Camera);
		add(camera);

		globalSystem = std::make_shared<MT::ParticleSystem>();
		globalSystem->name = starSystemId;
		globalSystem->setSize(5000.0, 5000.0);
		globalSystem->emitImmediatelyWithFactory(1000, std::make_shared<ParticleSpaceBackgroundParticleFactory>());
		add(globalSystem);
	}

	void TestSpaceScene::onChildrenHydrated()
	{
		globalSystem = findChildWithName<MT::ParticleSystem>(starSystemId);
		MT::NumberHelper::seedRng(getSceneName());
		globalSystem->emitImmediatelyWithFactory(3000, std::make_shared<ParticleSpaceBackgroundParticleFactory>());
	}

	void TestSpaceScene::onEnterFrame(double frameTime)
	{
	}

	void TestSpaceScene::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_ESCAPE:
			findFirstInParentChain<MT::SceneContainer>()->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
			break;

		case SDL_SCANCODE_1:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 10.0);
			break;

		case SDL_SCANCODE_2:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 1.0);
			break;

		case SDL_SCANCODE_3:
			modules->time->changeTimeFactorForScope(MT::TimeScope::Game, 0.25);
			break;
		}
	}
}
