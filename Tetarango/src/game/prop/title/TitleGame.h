#pragma once

#include <vector>
#include "ui/renderable/element/Text.h"
#include "ui/renderable/container/Container.h"
#include "engine/module/animation/Transition.h"
#include "engine/module/animation/INotifyOnTransition.h"

namespace AWGame
{
	class TitleGame : public AW::Container, public AW::INotifyOnTransition
	{
		std::shared_ptr<AW::Text> primaryTitle;
		std::vector<std::shared_ptr<AW::Text>> titles;

		std::shared_ptr<AW::Transition> introTransition;
		std::shared_ptr<AW::Transition> continuousTransition;

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
