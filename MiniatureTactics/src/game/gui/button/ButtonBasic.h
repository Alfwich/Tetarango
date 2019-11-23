#pragma once

#include <string>
#include "ui/renderable/element/Animated.h"
#include "ui/renderable/element/Text.h"
#include "IButtonListener.h"
#include "gui/BaseGui.h"

namespace MTGame
{

	class ButtonBasic : public BaseGui, public MT::Animated
	{
		std::string text;
		std::shared_ptr<MT::Text> label;
		bool isHovering = false, isPressed = false;
	public:
		ButtonBasic();

		void setText(std::string text);

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onDetach();
		void checkIsHovering(int x, int y);
		void onMouseButton(MT::MouseButton button, bool pressed);
		void onEnterFrame(double frameTime);

		std::weak_ptr<ApplicationObject> clickListener;
	};

}
