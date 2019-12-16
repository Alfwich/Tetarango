#pragma once

#include <random>
#include <sstream>
#include <algorithm>
#include "GameImports.h"
#include "GameConfig.h"
#include "ui/renderable/element/Rectangle.h"
#include "engine/module/display/DisplayRoot.h"
#include "engine/Application.h"

namespace AWGame
{

	class GameApplication : public AWCore::Application
	{
		std::shared_ptr<AWCore::SceneContainer> masterSceneContainer;

	public:
		void onInit();
		void onPrimeSerialization();
		void onReady();
		void onCleanup();

		bool onFailedToProvisionScreen();
		void onProvisionedScreen();
	};
}
