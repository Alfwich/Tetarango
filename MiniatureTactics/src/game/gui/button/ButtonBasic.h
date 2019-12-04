#pragma once

#include <string>
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/element/Text.h"
#include "IButtonListener.h"
#include "gui/BaseGui.h"

namespace MTGame
{

	class ButtonBasic : public BaseGui, public MT::NineSlice
	{
		std::string text;
		std::shared_ptr<MT::Text> label;
		bool isHovering = false, isPressed = false;
	public:
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle);
		ButtonBasic();

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onDetach();
		void checkIsHovering(int x, int y);
		void onMouseButton(MT::MouseButton button, bool pressed);
		void onEnterFrame(double frameTime);

		std::weak_ptr<ApplicationObject> clickListener;

		void setText(std::string text);
		std::string getText();
		void setEnabled(bool flag);
	};

}
