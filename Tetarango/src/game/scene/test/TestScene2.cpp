#include "TestScene2.h"

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

	TestSpace2::TestSpace2() : AW::Scene("test_space_scene")
	{
		setShouldRebuildOnLoad();
		registerGameObject<TestSpace2>();
	}

	void TestSpace2::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(AW::TimeScope::Game);

		modules->input->keyboard->registerKeys({ SDL_SCANCODE_ESCAPE, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3 }, weak_from_this());
	}

	void TestSpace2::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
	}

	void TestSpace2::onCreateChildren()
	{
		const auto mainGameMenu = std::make_shared<GameMainMenu>();
		mainGameMenu->zIndex = 20;
		mainGameMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		mainGameMenu->renderPositionMode = AW::RenderPositionMode::Absolute;
		mainGameMenu->visible = false;
		add(mainGameMenu);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(4.0, 1.0);
		camera->setDefaultZoomAndAnchorPoint(1.0, 0.0, 0.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		add(camera);

		globalSystem = std::make_shared<AW::ParticleSystem>();
		globalSystem->name = starSystemId;
		globalSystem->setSize(5000.0, 5000.0);
		globalSystem->emitImmediatelyWithFactory(1000, std::make_shared<ParticleSpaceBackgroundParticleFactory>());
		add(globalSystem);
	}

	void TestSpace2::onChildrenHydrated()
	{
		globalSystem = findChildWithName<AW::ParticleSystem>(starSystemId);
		AW::NumberHelper::seedRng(getSceneName());
		globalSystem->emitImmediatelyWithFactory(3000, std::make_shared<ParticleSpaceBackgroundParticleFactory>());
	}

	void TestSpace2::onEnterFrame(const double& frameTime)
	{
	}

	void TestSpace2::onKeyPressed(SDL_Scancode key)
	{
		switch (key)
		{
		case SDL_SCANCODE_ESCAPE:
			findFirstInParentChain<AW::SceneContainer>()->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
			break;

		case SDL_SCANCODE_1:
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 10.0);
			break;

		case SDL_SCANCODE_2:
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
			break;

		case SDL_SCANCODE_3:
			modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 0.25);
			break;
		}
	}
}
