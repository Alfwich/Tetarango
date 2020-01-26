#include "WorldTetarango.h"

#include "scene/game/SceneMainGame.h"
#include "ui/renderable/physic/Box.h"
#include "ui/renderable/physic/Line.h"
#include "ui/renderable/physic/Chain.h"
#include "prop/environment/Ground.h"

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


		ground = std::make_shared<Ground>();
		ground->name = "ground";
		contentContainer->add(ground);

		player = std::make_shared<Player>();
		player->toTopOf(ground);
		contentContainer->add(player);

		gameCamera = std::make_shared<GameCamera>();
		gameCamera->name = "gc";
		gameCamera->target = player;
		gameCamera->listener = weakPtr();
		contentContainer->add(gameCamera);

		environment = std::make_shared<Environment>();
		environment->name = "env";
		environment->setLengthOfDayInSeconds(60.0);
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
		ground = findChildWithName<Ground>("ground");
	}

	void SceneWorldTetarango::onCameraUpdate()
	{
		environment->setParallaxAmount(-gameCamera->getScreenAnchorX(), -gameCamera->getScreenAnchorY());
		ground->updateChunksForNewWorldCenter(gameCamera->getScreenAnchorX());
		setColor(environment->getEnvironmentColor());
	}
}
