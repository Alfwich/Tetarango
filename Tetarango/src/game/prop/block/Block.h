#pragma once

#include "ui/renderable/element/Animated.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/BaseProp.h"
#include "engine/module/animation/Transition.h"
#include "engine/module/animation/INotifyOnTransition.h"

namespace AWGame
{
	class Block : public AW::Animated, public BaseProp, public AW::INotifyOnTransition
	{
		std::shared_ptr<AW::Transition> blockTransition;
	public:
		Block();
		int blockX = 0, blockY = 0;
		bool hasSettled = false;

		void onTransitionAnimationFrame(double position);

		void onLoadResources();
		void onBindShaders();

		void onInitialAttach();
		void onCreateChildren();
		std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
	};
}
