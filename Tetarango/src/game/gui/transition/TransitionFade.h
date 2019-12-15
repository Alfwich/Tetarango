#pragma once

#include <memory>
#include "gui/BaseGui.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Rectangle.h"
#include "engine/module/animation/Animation.h"
#include "engine/module/animation/INotifyOnTransition.h"
#include "INotifyOnFade.h"

namespace AWGame
{

	class TransitionFade : public BaseGui, public MT::Container, public MT::INotifyOnTransition
	{
		std::shared_ptr<MT::Transition> fadeTransition;
		std::shared_ptr<MT::Rectangle> fadeRectangle;

	public:

		TransitionFade();

		void onInitialAttach();
		void onCreateChildren();
		void onChildrenHydrated();
		void onLayoutChildren();

		std::weak_ptr<INotifyOnFade> listener;

		void setDuration(double duration);
		double getDuration();

		void fadeOut();
		void fadeOutImmediately();

		void fadeIn();
		void fadeInImmediately();

		void onTransitionCompleted(int transitionId);
	};

}
