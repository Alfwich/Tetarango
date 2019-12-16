#pragma once

#include <string>
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"
#include "gui/IGuiListener.h"
#include "gui/BaseGui.h"

namespace AWGame
{

	class ScrollBarBasic : public BaseGui, public AWCore::Container
	{
		int startMousePositionOnClick = 0;
		double startMousePositionScrollPosition = 0.0;
		bool isHovering = false, isPressed = false, wasPressed = false;
		std::shared_ptr<AWCore::NineSlice> background;
		std::shared_ptr<AWCore::NineSlice> scroller;
		std::shared_ptr<AWCore::Transition> scrollerTransition;
		void updateScrollerPosition(bool instant = false);

	public:
		ScrollBarBasic();

		void onLoadResources();

		void setScrollerHeight(double height);
		double getScrollerHeight();

		void setHorizontal(bool flag);
		bool getHorizontal();

		void setScrollPositionInstantly(double pos);
		void setScrollPosition(double pos);
		double getScrollPosition();

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onDetach();
		void onLayoutChildren();
		void checkIsHovering(int x, int y);
		void onMouseButton(AWCore::MouseButton button, bool pressed);
		void onEnterFrame(double frameTime);

		void setEnabled(bool flag);
	};

}

