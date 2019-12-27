#include "TestScene.h"

#include <random>
#include "GameImports.h"
#include "ui/renderable/element/DisplayBuffer.h"

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
				SDL_SCANCODE_2,
				SDL_SCANCODE_3,
				SDL_SCANCODE_LEFT,
				SDL_SCANCODE_RIGHT,
				SDL_SCANCODE_UP,
				SDL_SCANCODE_LEFTBRACKET,
				SDL_SCANCODE_RIGHTBRACKET,
				SDL_SCANCODE_UP,
				SDL_SCANCODE_DOWN
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AW::MouseButton::Left, weak_from_this());

		iterTimer = modules->time->createTimer();
		iterTimer->start();

		updateTimer = modules->time->createTimer();
		updateTimer->start();
	}

	void TestScene::onAttach()
	{
		modules->time->changeTimeFactorForScope(AW::TimeScope::Game, 1.0);
	}

	void TestScene::onCreateChildren()
	{
		setSize(modules->screen->getWidth(), modules->screen->getHeight());
		topLeftAlignSelf();

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
		camera->setDefaultsAndReset(1.0, 0.0, 0.0);
		camera->setZoomAnchorPointOnScreen(modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		add(camera);

		red = std::make_shared<ScrollBarBasic>();
		red->renderPositionMode = AW::RenderPositionMode::Absolute;
		red->setSize(30, 500);
		red->scrollListener = shared_from_this();
		red->setColor(192, 0, 0);
		red->toInnerLeftIn(this);
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

		const auto background = std::make_shared<AW::Rectangle>();
		background->setVertexShader(modules->shader->getShader({ "v-default" }));
		background->setFragmentShader(modules->shader->getShader({ "f-mandelbrot" }));
		background->getVertexShader()->setFloatV2IUParam("vTranslate", 0, 0);
		background->getVertexShader()->setFloatIUParam("vScale", 1.0);
		background->getFragmentShader()->setFloatIUParam("iter", currentIters);
		background->getFragmentShader()->setFloatV3IUParam("fColor", 1.0, 1.0, 1.0);
		background->setColor(AW::Color::white());
		background->setSize(1200, 800);
		background->centerWithin(this);
		background->zIndex = -1;
		obj1 = background;

		const auto cached = std::make_shared<AW::DisplayBuffer>();
		cached->setClearColor(64, 0, 0);
		cached->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		cached->centerWithin(this);
		cached->zIndex = -1;
		add(cached);
		cached->add(background);
		obj2 = cached;
	}

	void TestScene::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene::onEnterFrame(const double& deltaTime)
	{
		if ((itersIncPressed || itersDecPressed) && iterTimer->isAboveThresholdAndRestart(10))
		{
			if (itersIncPressed)
			{
				currentIters = AW::NumberHelper::clamp(currentIters + 1.0, 0.0, 1000.0);
				obj1->getFragmentShader()->setFloatIUParam("iter", currentIters);
				infoLabel->setText("Iters: " + std::to_string((int)currentIters));
				obj2->markDirty();
			}
			else if (itersDecPressed)
			{
				currentIters = AW::NumberHelper::clamp(currentIters - 1.0, 0.0, 1000.0);
				obj1->getFragmentShader()->setFloatIUParam("iter", currentIters);
				infoLabel->setText("Iters: " + std::to_string((int)currentIters));
				obj2->markDirty();
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

		if (key == SDL_SCANCODE_3)
		{
			obj2->renderMode = AW::RenderMode::ChildrenOnly;
			obj1->getVertexShader()->setFloatV2IUParam("vTranslate", 0, 0);
			obj1->getVertexShader()->setFloatIUParam("vScale", 1.0);
		}

		if (key == SDL_SCANCODE_LEFTBRACKET && isPressed)
		{
			const auto vShader = obj1->getVertexShader();
			const auto currentZoom = vShader->getFloatIUParam("vScale");
			vShader->setFloatIUParam("vScale", currentZoom / 2.0);
			obj2->markDirty();
		}

		if (key == SDL_SCANCODE_RIGHTBRACKET && isPressed)
		{
			const auto vShader = obj1->getVertexShader();
			const auto currentZoom = vShader->getFloatIUParam("vScale");
			vShader->setFloatIUParam("vScale", currentZoom * 2.0);
			obj2->markDirty();
		}

		const auto vShader = obj1->getVertexShader();
		const auto currentZoom = vShader->getFloatIUParam("vScale");
		const auto moveDelta = 10.0 / currentZoom;
		if (key == SDL_SCANCODE_LEFT && isPressed)
		{
			const auto vShader = obj1->getVertexShader();
			const auto currentPos = vShader->getFloatV2IUParam("vTranslate");
			vShader->setFloatV2IUParam("vTranslate", std::get<0>(currentPos) + moveDelta, std::get<1>(currentPos));
			obj2->markDirty();
		}

		if (key == SDL_SCANCODE_RIGHT && isPressed)
		{
			const auto vShader = obj1->getVertexShader();
			const auto currentPos = vShader->getFloatV2IUParam("vTranslate");
			vShader->setFloatV2IUParam("vTranslate", std::get<0>(currentPos) - moveDelta, std::get<1>(currentPos));
			obj2->markDirty();
		}

		if (key == SDL_SCANCODE_UP && isPressed)
		{
			const auto vShader = obj1->getVertexShader();
			const auto currentPos = vShader->getFloatV2IUParam("vTranslate");
			vShader->setFloatV2IUParam("vTranslate", std::get<0>(currentPos), std::get<1>(currentPos) + moveDelta);
			obj2->markDirty();
		}

		if (key == SDL_SCANCODE_DOWN && isPressed)
		{
			const auto vShader = obj1->getVertexShader();
			const auto currentPos = vShader->getFloatV2IUParam("vTranslate");
			vShader->setFloatV2IUParam("vTranslate", std::get<0>(currentPos), std::get<1>(currentPos) - moveDelta);
			obj2->markDirty();
		}
	}

	void TestScene::onScrollBarScroll(int id, double pos)
	{
		auto currentColor = obj1->getFragmentShader()->getFloatV3IUParam("fColor");
		if (id == red->getId())
		{
			obj1->getFragmentShader()->setFloatV3IUParam("fColor", 1.0 - pos, std::get<1>(currentColor), std::get<2>(currentColor));
			obj2->markDirty();
		}

		if (id == green->getId())
		{
			obj1->getFragmentShader()->setFloatV3IUParam("fColor", std::get<0>(currentColor), 1.0 - pos, std::get<2>(currentColor));
			obj2->markDirty();
		}

		if (id == blue->getId())
		{
			obj1->getFragmentShader()->setFloatV3IUParam("fColor", std::get<0>(currentColor), std::get<1>(currentColor), 1.0 - pos);
			obj2->markDirty();
		}
	}

	void TestScene::onCameraUpdate()
	{
		const auto vShader = obj1->getVertexShader();

		vShader->setFloatV2IUParam("vTranslate", -camera->getX(), -camera->getY());
		//vShader->setFloatIUParam("vScale", camera->getZoom());
		obj1->setScale(camera->getZoom());

		if (updateTimer->isAboveThresholdAndRestart(16))
		{
			obj2->markDirty();
		}
	}
}
