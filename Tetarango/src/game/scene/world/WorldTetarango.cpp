#include "WorldTetarango.h"

#include "scene/game/SceneMainGame.h"
#include "ui/renderable/physic/Box.h"
#include "ui/renderable/physic/Line.h"
#include "ui/renderable/physic/Chain.h"

namespace AWGame
{
	SceneWorldTetarango::SceneWorldTetarango() : BaseScene(SceneGame::WorldTetarango)
	{
		GORegister(SceneWorldTetarango);
	}

	void SceneWorldTetarango::onAttach()
	{
		modules->physic->setWorldGravity(0);
		modules->physic->setWorldFps(0, 120);

		const auto mainGameScene = findFirstInParentChain<SceneMainGame>();
		if (mainGameScene != nullptr) mainGameScene->enableMenu();
	}

	void SceneWorldTetarango::onCreateChildren()
	{
		const auto contentContainer = std::make_shared<AW::Container>();
		contentContainer->name = "cc";
		add(contentContainer);

		const auto ground = std::make_shared<AW::Chain>();
		ground->setDynamic(false);
		for (auto i = -60; i < 60; ++i)
		{
			ground->addWorldPoint(i * 0.5, AW::NumberHelper::random(-0.15, 0.15));
		}
		ground->centerBalancePoints();
		contentContainer->add(ground);

		const auto groundCover = std::make_shared<AW::Polygon>();
		const auto pts = ground->getScreenPoints();
		groundCover->addScreenPoint(-15000, 10000);
		for (const auto pt : pts)
		{
			groundCover->addScreenPoint(pt.x, pt.y);
		}
		groundCover->addScreenPoint(15000, 10000);
		contentContainer->add(groundCover);

		player = std::make_shared<Player>();
		player->toTopOf(ground, 0, 1);
		contentContainer->add(player);

		gameCamera = std::make_shared<GameCamera>();
		gameCamera->name = "gc";
		gameCamera->target = player;
		gameCamera->listener = weakPtr();
		contentContainer->add(gameCamera);

		environment = std::make_shared<Environment>();
		environment->name = "env";
		environment->setLengthOfDayInSeconds(300.0);
		environment->zIndex = -1;
		add(environment);
	}

	void SceneWorldTetarango::onLayoutChildren()
	{
		const auto contentContainer = findChildWithName<AW::Container>("cc");
		if (contentContainer != nullptr)
		{
			contentContainer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		}

		environment->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		environment->topLeftAlignSelf();
		environment->layout();
	}

	void SceneWorldTetarango::onChildrenHydrated()
	{
		gameCamera = findChildWithName<GameCamera>("gc");
		gameCamera->listener = weakPtr();
		environment = findChildWithName<Environment>("env");
	}

	void SceneWorldTetarango::onCameraUpdate()
	{
		environment->setParallaxAmount(-gameCamera->getScreenAnchorX(), -gameCamera->getScreenAnchorY());
		setColor(environment->getEnvironmentColor());
	}
}
