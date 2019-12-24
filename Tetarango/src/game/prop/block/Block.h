#pragma once

#include "ui/renderable/element/Animated.h"
#include "engine/module/SystemModuleBundle.h"
#include "prop/BaseProp.h"
#include "engine/module/animation/Transition.h"
#include "engine/module/animation/INotifyOnTransition.h"

namespace AWGame
{
	class Block : public AW::Element, public BaseProp, public AW::INotifyOnTransition
	{
		double energy = 1.0;
	public:
		Block();
		int blockX = 0, blockY = 0;
		bool hasSettled = false;

		double addEnergy(double amount);
		double removeEnergy(double amount);
		void setEnergy(double amount);
		double getEnergy();

		void onLoadResources();
		void onBindShaders();

		void onInitialAttach();
		std::shared_ptr<AW::SerializationClient> doSerialize(AW::SerializationHint hint);
	};
}
