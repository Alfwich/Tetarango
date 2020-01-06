#pragma once

#include "ui/renderable/element/Animated.h"
#include "ui/renderable/element/Text.h"
#include "gui/IGuiListener.h"
#include "gui/BaseGui.h"

namespace AWGame
{

	class CheckBoxBasic: public BaseGui, public AW::Container
	{
		std::string text;
		std::shared_ptr<AW::Animated> checkbox;
		std::shared_ptr<AW::Text> label;
		bool isHovering = false, isPressed = false;

	public:
		CheckBoxBasic();
		CheckBoxBasic(GuiButton configuration);

		void onLoadResources();

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onLayoutChildren();
		void onDetach();
		void checkIsHovering(int x, int y);
		void onMouseButton(AWMouseButton button, bool pressed);
		void onEnterFrame(const double& frameTime);

		std::weak_ptr<GameObject> clickListener;

		void setText(std::string text);
		std::string getText();

		bool getChecked();
		void setChecked(bool isChecked);
		void setEnabled(bool flag);
	};

}

