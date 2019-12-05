#pragma once

#include <string>
#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"
#include "gui/IGuiListener.h"
#include "gui/BaseGui.h"

namespace MTGame
{

	class ScrollBarBasic : public BaseGui, public MT::Container
	{
		bool isHovering = false, isPressed = false;
	public:
		static void loadResources(std::shared_ptr<MT::SystemModuleBundle> bundle);
		ScrollBarBasic();

		void onCreateChildren();
		void onChildrenHydrated();
		void onInitialAttach();
		void onDetach();
		void checkIsHovering(int x, int y);
		void onMouseButton(MT::MouseButton button, bool pressed);
		void onEnterFrame(double frameTime);

		void setEnabled(bool flag);
	};

}

