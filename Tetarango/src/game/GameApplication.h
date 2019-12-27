#pragma once

#include <random>
#include <sstream>
#include <algorithm>
#include "GameImports.h"
#include "GameConfig.h"
#include "ui/renderable/primitive/Rectangle.h"
#include "engine/module/display/DisplayRoot.h"
#include "engine/Application.h"
#include "ui/renderable/element/DisplayBuffer.h"

#include "debug/monitor/DebugMonitor.h"

namespace AWGame
{

	class GameApplication : public AW::Application
	{
		std::shared_ptr<AW::DebugMonitor> debugMonitor;
		std::shared_ptr<AW::SceneContainer> masterSceneContainer;
		std::shared_ptr<AW::DisplayBuffer> screenBuffer;

	public:
		void onInit();
		void onPrimeSerialization();
		void onLoadResources();
		void onReady();
		void onCleanup();

		bool onFailedToProvisionScreen();
		void onProvisionedScreen();
	};
}
