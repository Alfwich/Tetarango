#pragma once

#include "ui/renderable/element/Animated.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/BaseProp.h"

namespace MTGame
{
	class Block : public MT::Animated, public BaseProp
	{
		static bool isLoaded;

	public:

		static void primeBlocks();
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle);

		void onInitialAttach();
		void onCreateChildren();
	};
}
