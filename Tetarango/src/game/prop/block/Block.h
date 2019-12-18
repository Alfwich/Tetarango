#pragma once

#include "ui/renderable/element/Animated.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/BaseProp.h"

namespace AWGame
{
	class Block : public AW::Animated, public BaseProp
	{
	public:
		Block();
		int blockX = 0, blockY = 0;
		bool hasSettled = false;

		void onLoadResources();
		void onBindShaders();
		void onInitialAttach();
		void onCreateChildren();
		std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
	};
}
