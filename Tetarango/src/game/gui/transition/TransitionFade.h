#pragma once

#include <memory>
#include "gui/BaseGui.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Element.h"
#include "engine/module/animation/Animation.h"
#include "engine/module/animation/INotifyOnTransition.h"
#include "INotifyOnFade.h"

namespace AWGame
{

	class TransitionFade : public BaseGui, public AW::Container, public AW::INotifyOnTransition
	{
		std::shared_ptr<AW::Transition> fadeTransition;
		std::shared_ptr<AW::Element> fadeRectangle;

		bool shouldFadeIn = false;

		void setTransitionShader();

	public:

		TransitionFade();

		void onLoadResources();

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
		void onTransitionAnimationFrame(double p);
	};

}
