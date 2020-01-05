#include "WorldTetarango.h"

#include "scene/game/SceneMainGame.h"
#include "prop/physic/Box.h"
#include "actor/player/Player.h"
#include "gui/camera/GameCamera.h"
#include "prop/physic/Line.h"
#include "prop/physic/Chain.h"

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
		contentContainer->setScreenSize(modules->screen->getWidth(), modules->screen->getHeight());
		contentContainer->topLeftAlignSelf();
		add(contentContainer);

		const auto ground = std::make_shared<Chain>();
		ground->setDynamic(false);
		for (auto i = -30; i < 30; ++i)
		{
			ground->addWorldPoint(i, AW::NumberHelper::random(-1, 1));
		}
		ground->centerBalancePoints();
		ground->centerAlignWithin(contentContainer);
		contentContainer->add(ground);

		const auto player = std::make_shared<Player>();
		player->centerAlignWithin(contentContainer, 0.0, -100.0);
		contentContainer->add(player);

		const auto gameCamera = std::make_shared<GameCamera>();
		gameCamera->target = player;
		contentContainer->add(gameCamera);
	}

	void SceneWorldTetarango::onChildrenHydrated()
	{
	}
}
