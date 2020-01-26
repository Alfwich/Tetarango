#pragma once

#include "scene/BaseScene.h"
#include "gui/camera/GameCamera.h"
#include "prop/environment/Environment.h"
#include "prop/environment/Ground.h"
#include "actor/player/Player.h"

namespace AWGame
{
	class SceneWorldTetarango : public BaseScene, public AW::ICameraListener
	{
		std::shared_ptr<Player> player;
		std::shared_ptr<GameCamera> gameCamera;
		std::shared_ptr<Environment> environment;
		std::shared_ptr<Ground> ground;

	public:
		SceneWorldTetarango();

		void onAttach();

		void onCreateChildren();
		void onLayoutChildren();
		void onChildrenHydrated();

		void onCameraUpdate();
	};
}
