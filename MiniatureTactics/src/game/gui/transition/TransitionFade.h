#pragma once

#include <memory>
#include "gui/BaseGui.h"
#include "ui/renderable/element/Rectangle.h"
#include "engine/module/animation/Animation.h"
#include "engine/module/animation/INotifyOnTransition.h"
#include "INotifyOnFade.h"

namespace MTGame
{

	class TransitionFade : public BaseGui, public MT::Rectangle, public MT::INotifyOnTransition
	{
		std::shared_ptr<MT::Transition> fadeTransition;

	public:

		TransitionFade();

		void onInitialAttach();

		std::weak_ptr<INotifyOnFade> listener;

		void setDuration(double duration);

		void fadeOut();
		void fadeOutImmediately();

		void fadeIn();
		void fadeInImmediately();

		void onTransitionCompleted(int transitionId);
	};

}
