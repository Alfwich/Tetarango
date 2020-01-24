#include "WorldTetarango.h"

#include "scene/game/SceneMainGame.h"
#include "ui/renderable/physic/Box.h"
#include "ui/renderable/physic/Line.h"
#include "ui/renderable/physic/Chain.h"
#include "prop/environment/Ground.h"

namespace
{
	const auto leftBoundParamName = "swt-l-b";
	const auto rightBoundParamName = "swt-r-b";
	const auto chunkExpandFactor = 1;
	const auto chunkExpandDistanceThreshold = 1000.0;
}

namespace AWGame
{
	SceneWorldTetarango::SceneWorldTetarango() : BaseScene(SceneGame::WorldTetarango)
	{
		GORegister(SceneWorldTetarango);
	}

	void SceneWorldTetarango::expandWorldIfNeeded()
	{
		const auto leftBound = serializationClient->getDouble(leftBoundParamName);
		const auto rightBound = serializationClient->getDouble(rightBoundParamName);
		const auto leftD = std::abs(leftBound * 1000.0 - gameCamera->getScreenAnchorX());
		const auto rightD = std::abs(rightBound * 1000.0 - gameCamera->getScreenAnchorX());

		if (leftD < chunkExpandDistanceThreshold)
		{
			const auto contentContainer = findChildWithName<AW::Container>("cc");
			for (auto i = leftBound - chunkExpandFactor; i < leftBound; ++i)
			{
				const auto ground = std::make_shared<Ground>();
				ground->setScreenPosition(i * 1000.0, 0.0);
				contentContainer->add(ground);
			}

			serializationClient->setDouble(leftBoundParamName, leftBound - chunkExpandFactor);
		}

		if (rightD < chunkExpandDistanceThreshold)
		{
			const auto contentContainer = findChildWithName<AW::Container>("cc");
			for (auto i = rightBound + 1; i < rightBound + chunkExpandFactor + 1; ++i)
			{
				const auto ground = std::make_shared<Ground>();
				ground->setScreenPosition(i * 1000.0, 0.0);
				contentContainer->add(ground);
			}

			serializationClient->setDouble(rightBoundParamName, rightBound + chunkExpandFactor);
		}

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

		player = std::make_shared<Player>();

		{
			const auto ground = std::make_shared<Ground>();
			ground->setScreenPosition(0.0, 0.0);
			player->toTopOf(ground, 0, 1);
			contentContainer->add(ground);
		}
		contentContainer->add(player);

		for (auto i = -chunkExpandFactor; i < chunkExpandFactor + 1; ++i)
		{
			if (i == 0) continue;

			const auto ground = std::make_shared<Ground>();
			ground->setScreenPosition(i * 1000.0, 0.0);
			contentContainer->add(ground);
		}

		serializationClient->setDouble(leftBoundParamName, -chunkExpandFactor);
		serializationClient->setDouble(rightBoundParamName, chunkExpandFactor);

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
		expandWorldIfNeeded();
	}
}
