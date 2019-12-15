#pragma once

#include <vector>
#include "ui/renderable/element/Text.h"
#include "ui/renderable/container/Container.h"
#include "engine/module/animation/Transition.h"
#include "engine/module/animation/INotifyOnTransition.h"

namespace MTGame
{
	class TitleGame : public MT::Container, public MT::INotifyOnTransition
	{
		std::shared_ptr<MT::Text> primaryTitle;
		std::vector<std::shared_ptr<MT::Element>> titles;

	public:
		TitleGame();

		void setFontSize(int fontSize);
		int getFontSize();

		void onCreateChildren();
		void onLayoutChildren();
	};
}
