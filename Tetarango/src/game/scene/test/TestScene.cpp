#include "TestScene.h"

#include "scene/BaseScene.h"
#include "ui/renderable/element/DisplayBuffer.h"
#include "ui/renderable/element/Polygon.h"

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
		GORegister(TestScene);
	}

	void TestScene::onLoadResources()
	{
		modules->lua->registerObjectImplementation("res/lua/game/object/color-changer.lua", "color-changer");
	}

	void TestScene::onInitialAttach()
	{
		enableEnterFrame();
		setTimeScope(AW::TimeScope::Game);

		modules->input->keyboard->registerKeys({
				AWKey::ESCAPE,
				AWKey::ONE,
				AWKey::TWO,
				AWKey::THREE,
				AWKey::FOUR,
				AWKey::FIVE,
				AWKey::SIX,
				AWKey::LEFT,
				AWKey::RIGHT,
				AWKey::UP,
				AWKey::LEFTBRACKET,
				AWKey::RIGHTBRACKET,
				AWKey::UP,
				AWKey::DOWN
			}
		, weak_from_this());
		modules->input->mouse->registerMouseButton(AWMouseButton::Left, weak_from_this());

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
		const auto contentContainer = std::make_shared<AW::Rectangle>();
		contentContainer->name = "cc";
		contentContainer->renderColorMode = AW::RenderColorMode::Absolute;
		contentContainer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		contentContainer->topLeftAlignSelf();
		add(contentContainer);
		this->contentContainer = contentContainer;

		camera = std::make_shared<GameCamera>();
		camera->name = "camera";
		camera->setZoomLimits(16000000.0, 0);
		camera->setDefaultZoomAndAnchorPoint(1.0, modules->screen->getWidth() / 2.0, modules->screen->getHeight() / 2.0);
		camera->setTimeScope(AW::TimeScope::Camera);
		camera->listener = shared_from_this();
		contentContainer->add(camera);

		const auto cached = std::make_shared<AW::DisplayBuffer>();
		cached->setClearColor(128, 128, 128);
		cached->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		cached->centerWithin(contentContainer, 30.0, 30.0);
		cached->zIndex = -1;
		contentContainer->add(cached);
		obj2 = cached;

		const auto background = std::make_shared<AW::Rectangle>();
		background->setVertexShader(modules->shader->getShader({ "v-default" }));
		background->setFragmentShader(modules->shader->getShader({ "f-color", "f-mandelbrot" }));
		background->getFragmentShader()->setFloatIUParam("iter", currentIters);
		background->getFragmentShader()->setFloatV3IUParam("fColor", 1.0, 1.0, 1.0);
		background->setScreenSize(1200, 800);
		background->centerWithin(cached, 100.0);
		background->zIndex = -1;
		cached->add(background);
		obj1 = background;

		const auto testR = std::make_shared<AW::Rectangle>();
		testR->setColor(AW::Color::red());
		testR->setScreenSize(25.0, 25.0);
		testR->centerAlignWithin(background, -10.0, -10.0);
		background->add(testR);

		const auto testR2 = std::make_shared<AW::Rectangle>();
		testR2->renderColorMode = AW::RenderColorMode::Absolute;
		testR2->setColor(AW::Color::green());
		testR2->setScreenSize(15.0, 15.0);
		testR2->centerAlignWithin(testR, 20.0, 20.0);
		testR->add(testR2);

		infoLabel = std::make_shared<AW::Text>();
		infoLabel->renderPositionMode = AW::RenderPositionMode::Absolute;
		infoLabel->setFont("console", 24);
		infoLabel->setTextColor(255, 64, 64);
		infoLabel->setText("Iters: " + std::to_string((int)currentIters));
		infoLabel->toInnerTopIn(contentContainer);
		add(infoLabel);

		red = std::make_shared<ScrollBarBasic>();
		red->renderPositionMode = AW::RenderPositionMode::Absolute;
		red->setScreenSize(30, 500);
		red->scrollListener = shared_from_this();
		red->setColor(192, 0, 0);
		red->toInnerLeftIn(contentContainer);
		add(red);

		for (int i = 0; i < 100; ++i)
		{
			const auto btn = std::make_shared<ButtonBasic>();
			btn->setText("Hello World!");
			btn->setScreenPosition(AW::NumberHelper::random(-1000.0, -2000.0), AW::NumberHelper::random(-1000.0, -2000.0));
			btn->setScale(AW::NumberHelper::random(0.5, 2.0));
			btn->setColor(AW::Color::random());
			contentContainer->add(btn);
		}

		green = std::make_shared<ScrollBarBasic>();
		green->renderPositionMode = AW::RenderPositionMode::Absolute;
		green->setScreenSize(30, 500);
		green->scrollListener = shared_from_this();
		green->setColor(0, 192, 0);
		green->toRightOf(red, 2.0);
		add(green);

		blue = std::make_shared<ScrollBarBasic>();
		blue->renderPositionMode = AW::RenderPositionMode::Absolute;
		blue->setScreenSize(30, 500);
		blue->scrollListener = shared_from_this();
		blue->setColor(0, 0, 192);
		blue->toRightOf(green, 2.0);
		add(blue);

		xF = std::make_shared<ScrollBarBasic>();
		xF->renderPositionMode = AW::RenderPositionMode::Absolute;
		xF->setScreenSize(30, 500);
		xF->toRightOf(blue, 4.0);
		add(xF);
		xF->setScrollPosition(1.0);

		yF = std::make_shared<ScrollBarBasic>();
		yF->renderPositionMode = AW::RenderPositionMode::Absolute;
		yF->setScreenSize(30, 500);
		yF->toRightOf(xF, 2.0);
		add(yF);
		yF->setScrollPosition(1.0);

		{
			const auto poly = std::make_shared<AW::Polygon>();

			poly->setColor(AW::Color::red());
			poly->setTexture("noise-solid-512");
			poly->addScreenPoint(0.0, 0.0);
			poly->addScreenPoint(100.0, 0.0);
			poly->addScreenPoint(100.0, 500.0);
			poly->addScreenPoint(0.0, 500.0);

			contentContainer->add(poly);
			obj4 = poly;
		}

		for (auto i = 0; i < 100; ++i)
		{
			const auto poly = std::make_shared<AW::Polygon>();
			poly->setLuaImplementationAndEnable("color-changer");
			const auto numP = AW::NumberHelper::randomInt(3, 20);
			for (auto j = 0; j < numP; ++j)
			{
				poly->addScreenPoint(AW::NumberHelper::random(-50, 50), AW::NumberHelper::random(-50, 50));
			}
			contentContainer->add(poly);
		}

		layoutExampleRects.clear();
		for (int i = 0; i < 80; ++i)
		{
			const auto exR = std::make_shared<AW::Rectangle>();
			exR->setAlpha(0.5);
			layoutExampleRects.push_back(exR);
			contentContainer->add(exR);
		}
	}

	void TestScene::onChildrenHydrated()
	{
		camera = findChildWithName<GameCamera>("camera");
	}

	void TestScene::onEnterFrame(const double& deltaTime)
	{
		if (obj4 == nullptr) return;
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

		obj4->rotateScreen(deltaTime / 1000.0 * 45.0);

		for (const auto layoutR : layoutExampleRects)
		{
			layoutR->setScreenPosition(10000, 10000);
			layoutR->setScreenSize(200, 200);
		}

		int i = 0;
		auto example1 = layoutExampleRects[i++];
		example1->layoutSpace = AW::LayoutSpace::Screen;
		example1->setWorldPositionAndSize(15, 5, 5, 5);
		example1->setColor(255, 0, 0);

		auto example2 = layoutExampleRects[i++];
		example2->layoutSpace = AW::LayoutSpace::Screen;
		example2->setWorldSize(5, 5);
		example2->setColor(255, 128, 0);
		example2->toLeftOf(example1, 600);

		auto target = example1;
		auto space = AW::LayoutSpace::Screen;
		layoutMotionTime += deltaTime / 1000.0;
		auto exC = AW::Color(0, 128, 128);
		auto offsetX = std::cosf(layoutMotionTime * AW::NumberHelper::PI * 2.0) * 100.0 * xF->getScrollPosition(), offsetY = std::sinf(layoutMotionTime * AW::NumberHelper::PI * 2.0) * 100.0 * yF->getScrollPosition();

		for (int j = 0; j < 2; ++j)
		{
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toTopOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toTopLeftOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toTopRightOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toRightOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toRightTopOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toRightBottomOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toLeftOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toLeftTopOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toLeftBottomOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toBottomOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toBottomLeftOf(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toBottomRightOf(target, offsetX, offsetY);
			}


			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerTopIn(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerTopLeftIn(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerTopRightIn(target, offsetX, offsetY);
			}

			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerLeftIn(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->matchPosition(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerRightIn(target, offsetX, offsetY);
			}

			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerBottomIn(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerBottomRightIn(target, offsetX, offsetY);
			}
			{
				const auto rect = layoutExampleRects[i++];
				rect->setColor(exC);
				rect->layoutSpace = space;
				rect->toInnerBottomLeftIn(target, offsetX, offsetY);
			}

			offsetX /= 200.0;
			offsetY /= 200.0;
			space = AW::LayoutSpace::World;
			target = example2;
			exC.r = 64;
			exC.b = 255;
		}
	}

	void TestScene::onKeyPressed(AWKey key)
	{
		if (key == AWKey::ESCAPE)
		{
			destroyChildren();
			layoutExampleRects.clear();
			const auto applicationSceneContainer = findFirstInParentChain<AW::SceneContainer>();
			applicationSceneContainer->transitionToScene(BaseScene::sceneToStr(SceneGame::SavedGamesMenu));
		}
	}

	void TestScene::onKey(AWKey key, bool isPressed)
	{
		if (key == AWKey::ONE)
		{
			itersDecPressed = isPressed;
		}

		if (key == AWKey::TWO)
		{
			itersIncPressed = isPressed;
		}

		if (key == AWKey::THREE && isPressed)
		{
			obj2->renderMode = obj2->renderMode == AW::RenderMode::CachedElement ? AW::RenderMode::ChildrenOnly : AW::RenderMode::CachedElement;
			obj2->markDirty();
		}

		if (key == AWKey::FOUR && isPressed)
		{
			modules->network->httpGet("http://arthurwut.com/misc/merge.html", weakPtr<AW::INetworkListener>());
			modules->network->httpPost("http://arthurwut.com/misc/test.php", { {"hello", "world"}, {"lets", "go!" } }, weakPtr<AW::INetworkListener>());
		}

		if (key == AWKey::FIVE && isPressed)
		{
			modules->lua->registerObjectImplementation("res/lua/game/object/color-changer.lua", "color-changer");
			for (const auto poly: findChildWithName<GameObject>("cc")->getChildrenOfType<AW::Polygon>())
			{
				poly->setLuaImplementationAndEnable("color-changer");
			}
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
		if (updateTimer->isAboveThresholdAndRestart(16))
		{
			obj2->markDirty();
		}
	}

	void TestScene::onNetworkCallCompleted(int status, std::string& body)
	{
		std::cout << status << ", " << body << std::endl;
	}
}
