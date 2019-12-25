#include "TestScene.h"

#include <random>
#include "GameImports.h"

namespace
{
	const char* backButtonId = "back_button";
	const std::string player = "player";
}

namespace AWGame
{

	TestScene::TestScene() : AW::Scene("test_scene")
	{
		setShouldRebuildOnLoad();
		registerGameObject<TestScene>();
	}

	void TestScene::onLoadResources()
	{
	}

	void TestScene::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(AW::TimeScope::Game);

		modules->input->keyboard->registerKeys({
				SDL_SCANCODE_ESCAPE,
				SDL_SCANCODE_1,
				SDL_SCANCODE_2
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());

		iterTimer = modules->time->createTimer();
		iterTimer->start();
	}

	void TestScene::onAttach()
	{
		modules->screen->disableClear();
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
	}

	void TestScene::onDetach()
	{
		modules->screen->enableClear();
	}

	void TestScene::onCreateChildren()
	{
		const auto mainGameMenu = std::make_shared<GameMainMenu>();
		mainGameMenu->zIndex = 20;
		mainGameMenu->setPosition(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		mainGameMenu->renderPositionMode = AW::RenderPositionMode::Absolute;
		mainGameMenu->visible = false;
		add(mainGameMenu);

		infoLabel = std::make_shared<AW::Text>();
		infoLabel->renderPositionMode = AW::RenderPositionMode::Absolute;
		infoLabel->setFont("console", 24);
		infoLabel->setTextColor(255, 64, 64);
		infoLabel->setText("Iters: " + std::to_string((int)currentIters));
		infoLabel->topLeftAlignSelf(modules->screen->getWidth() / 2.0, 20.0);
		add(infoLabel);

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(16000000.0, 0);
		camera->setDefaultsAndReset(2.0, 0.0, 0.0);
		camera->setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		add(camera);
		{
			const auto background = std::make_shared<AW::Rectangle>();
			background->renderUpdateMode = AW::RenderUpdateMode::WhenDirty;
			background->setVertexShader(modules->shader->getShader({ "v-default" }));
			background->setFragmentShader(modules->shader->getShader({ "f-mandelbrot" }));
			background->getVertexShader()->setFloatV2IUParam("vTranslate", 0.0, 0.0);
			background->getFragmentShader()->setFloatIUParam("iter", currentIters);
			background->getFragmentShader()->setFloatV3IUParam("fColor", 1.0, 1.0, 1.0);
			background->setColor(AW::Color::white());
			background->setSize(600, 400);
			background->setPosition(0, 0);
			background->zIndex = -1;
			add(background);

			obj = background;
		}

		red = std::make_shared<ScrollBarBasic>();
		red->renderPositionMode = AW::RenderPositionMode::Absolute;
		red->setSize(30, 500);
		red->scrollListener = shared_from_this();
		red->setColor(192, 0, 0);
		red->topLeftAlignSelf(2.0, modules->screen->getHeight() / 2.0 - red->getHalfHeight());
		add(red);

		green = std::make_shared<ScrollBarBasic>();
		green->renderPositionMode = AW::RenderPositionMode::Absolute;
		green->setSize(30, 500);
		green->scrollListener = shared_from_this();
		green->setColor(0, 192, 0);
		green->toRightOf(red, 2.0);
		add(green);

		blue = std::make_shared<ScrollBarBasic>();
		blue->renderPositionMode = AW::RenderPositionMode::Absolute;
		blue->setSize(30, 500);
		blue->scrollListener = shared_from_this();
		blue->setColor(0, 0, 192);
		blue->toRightOf(green, 2.0);
		add(blue);
	}

	void TestScene::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene::onEnterFrame(const double& deltaTime)
	{
		if ((itersIncPressed || itersDecPressed) && iterTimer->isAboveThresholdAndRestart(100))
		{
			if (itersIncPressed)
			{
				currentIters = AW::NumberHelper::clamp(currentIters + 1.0, 0.0, 1000.0);
				obj->getFragmentShader()->setFloatIUParam("iter", currentIters);
				infoLabel->setText("Iters: " + std::to_string((int)currentIters));
				obj->markDirty();
			}
			else if (itersDecPressed)
			{
				currentIters = AW::NumberHelper::clamp(currentIters - 1.0, 0.0, 1000.0);
				obj->getFragmentShader()->setFloatIUParam("iter", currentIters);
				infoLabel->setText("Iters: " + std::to_string((int)currentIters));
				obj->markDirty();
			}
		}
	}

	void TestScene::onKeyPressed(SDL_Scancode key)
	{
		if (key == SDL_SCANCODE_ESCAPE)
		{
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::MainMenu));
		}
	}

	void TestScene::onKey(SDL_Scancode key, bool isPressed)
	{
		if (key == SDL_SCANCODE_1)
		{
			itersDecPressed = isPressed;
		}

		if (key == SDL_SCANCODE_2)
		{
			itersIncPressed = isPressed;
		}
	}

	void TestScene::onScrollBarScroll(int id, double pos)
	{
		auto currentColor = obj->getFragmentShader()->getFloatV3IUParam("fColor");
		if (id == red->getId())
		{
			obj->getFragmentShader()->setFloatV3IUParam("fColor", 1.0 - pos, std::get<1>(currentColor), std::get<2>(currentColor));
			obj->markDirty();
		}

		if (id == green->getId())
		{
			obj->getFragmentShader()->setFloatV3IUParam("fColor", std::get<0>(currentColor), 1.0 - pos, std::get<2>(currentColor));
			obj->markDirty();
		}

		if (id == blue->getId())
		{
			obj->getFragmentShader()->setFloatV3IUParam("fColor", std::get<0>(currentColor), std::get<1>(currentColor), 1.0 - pos);
			obj->markDirty();
		}
	}

	void TestScene::onCameraUpdate()
	{
		obj->markDirty();
	}
}
