#pragma once

#include "ui/renderable/element/Animated.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/BaseProp.h"

namespace MTGame
{
	class Block : public MT::Animated, public BaseProp
	{
	public:
		Block();
		int blockX = 0, blockY = 0;
		bool hasSettled = false;

		void onLoadResources();
		void onInitialAttach();
		void onCreateChildren();
		std::shared_ptr<MT::SerializationClient> doSerialize(MT::SerializationHint hint);
	};
}
