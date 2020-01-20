#include "WorldTetarango.h"

#include "scene/game/SceneMainGame.h"
#include "actor/player/Player.h"
#include "gui/camera/GameCamera.h"
#include "ui/renderable/physic/Box.h"
#include "ui/renderable/physic/Line.h"
#include "ui/renderable/physic/Chain.h"
#include "prop/environment/Environment.h"

namespace AWGame
{
	SceneWorldTetarango::SceneWorldTetarango() : BaseScene(SceneGame::WorldTetarango)
	{
		GORegister(SceneWorldTetarango);
	}

	void SceneWorldTetarango::onAttach()
	{
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
			ground->addWorldPoint(i * 1.0, AW::NumberHelper::random(-0.25, 0.25));
		}
		ground->centerBalancePoints();
		contentContainer->add(ground);

		const auto player = std::make_shared<Player>();
		player->toTopOf(ground, 0, 1);
		contentContainer->add(player);

		const auto gameCamera = std::make_shared<GameCamera>();
		gameCamera->target = player;
		contentContainer->add(gameCamera);

		const auto env = std::make_shared<Environment>();
		env->name = "env";
		env->zIndex = -1;
		add(env);
	}

	void SceneWorldTetarango::onLayoutChildren()
	{
		const auto contentContainer = findChildWithName<AW::Container>("cc");
		if (contentContainer != nullptr)
		{
			contentContainer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		}

		const auto env = findChildWithName<Environment>("env");
		if (env != nullptr)
		{
			env->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
			env->topLeftAlignSelf();
		}
	}

	void SceneWorldTetarango::onChildrenHydrated()
	{
	}
}
