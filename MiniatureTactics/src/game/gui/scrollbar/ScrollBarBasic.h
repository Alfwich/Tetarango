#pragma once

#include <string>
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"
#include "gui/IGuiListener.h"
#include "gui/BaseGui.h"

namespace MTGame
{

	class ScrollBarBasic : public BaseGui, public MT::Container
	{
		bool isHovering = false, isPressed = false;
		std::shared_ptr<MT::NineSlice> background;
		std::shared_ptr<MT::NineSlice> scroller;
	public:
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle);
		ScrollBarBasic();

		void setScrollerHeight(double height);
		double getScrollerHeight();

		void setScrollPosition(double pos);
		double getScrollPosition();

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onDetach();
		void onLayoutChildren();
		void checkIsHovering(int x, int y);
		void onMouseButton(MT::MouseButton button, bool pressed);
		void onEnterFrame(double frameTime);

		void setEnabled(bool flag);
	};

}

