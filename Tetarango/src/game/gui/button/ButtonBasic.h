#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/NineSlice.h"
#include "ui/renderable/element/Text.h"
#include "gui/IGuiListener.h"
#include "gui/BaseGui.h"

namespace AWGame
{

	class ButtonBasic : public BaseGui, public AW::Container
	{
		std::string text;
		std::shared_ptr<AW::Text> label;
		std::shared_ptr<AW::NineSlice> background;
		bool isHovering = false, isPressed = false;
	public:
		ButtonBasic();

		void onLoadResources();

		void onCreateChildren();
		void onLayoutChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onDetach();
		void checkIsHovering(int x, int y);
		void onMouseButton(AWMouseButton button, bool pressed);
		void onEnterFrame(const double& frameTime);

		void setText(std::string text);
		std::string getText();

		void setFontSize(int size);
		int getFontSize();

		void setEnabled(bool flag);
		bool getEnabled();
	};

}
