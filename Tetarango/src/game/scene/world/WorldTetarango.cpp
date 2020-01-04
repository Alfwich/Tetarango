#include "WorldTetarango.h"

#include "scene/game/SceneMainGame.h"
#include "prop/physic/Box.h"
#include "actor/player/Player.h"
#include "gui/camera/GameCamera.h"

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
		contentContainer->setSize(modules->screen->getWidth(), modules->screen->getHeight());
		contentContainer->topLeftAlignSelf();
		add(contentContainer);

		const auto ground = std::make_shared<Box>();
		ground->setDynamic(false);
		ground->setSize(2000.0, 100.0);
		ground->setColor(150, 75, 0);
		contentContainer->add(ground);

		const auto player = std::make_shared<Player>();
		player->setPosition(0.0, -200);
		contentContainer->add(player);

		const auto gameCamera = std::make_shared<GameCamera>();
		gameCamera->target = player;
		contentContainer->add(gameCamera);
	}

	void SceneWorldTetarango::onChildrenHydrated()
	{
	}
}
