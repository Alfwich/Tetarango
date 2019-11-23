#pragma once

#include <random>
#include <sstream>
#include <algorithm>
#include "GameImports.h"
#include "GameConfig.h"
#include "ui/renderable/element/Rectangle.h"
#include "engine/module/display/DisplayRoot.h"

namespace MTGame
{

	class GameApplication : public MT::Application
	{
		std::shared_ptr<MT::SceneContainer> masterSceneContainer;

	public:
		void onInit();
		void onLoadResources();
		void onPrimeSerialization();
		void onReady();
		void onCleanup();
	};
}
