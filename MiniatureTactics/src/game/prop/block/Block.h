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
		Block();
		int blockX = 0, blockY = 0;
		bool hasSettled = false;

		static void primeBlocks();
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle);

		void onInitialAttach();
		void onCreateChildren();
		std::shared_ptr<MT::SerializationClient> doSerialize(MT::SerializationHint hint);
	};
}
