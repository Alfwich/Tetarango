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
		double energy = 0.0;
	public:
		Block();
		int blockX = 0, blockY = 0;
		bool hasSettled = false;

		void addEnergy(double amount);
		double getEnergy();

		void onLoadResources();
		void onBindShaders();

		void onInitialAttach();
		void onCreateChildren();
		std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
	};
}
