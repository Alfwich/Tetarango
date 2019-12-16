#pragma once

#include <vector>
#include "ui/renderable/element/Text.h"
#include "ui/renderable/container/Container.h"
#include "engine/module/animation/Transition.h"
#include "engine/module/animation/INotifyOnTransition.h"

namespace AWGame
{
	class TitleGame : public AWCore::Container, public AWCore::INotifyOnTransition
	{
		std::shared_ptr<AWCore::Text> primaryTitle;
		std::vector<std::shared_ptr<AWCore::Text>> titles;

		std::shared_ptr<AWCore::Transition> introTransition;
		std::shared_ptr<AWCore::Transition> continuousTransition;

	public:
		TitleGame();

		void setFontSize(int fontSize);
		int getFontSize();

		void onInitialAttach();
		void onCreateChildren();
		void onLayoutChildren();

		void onTimeoutCalled();

		void onTransitionAnimationFrame(double position, int id);
		void onTransitionCompleted();
	};
}
