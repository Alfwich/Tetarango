#pragma once

#include "gui/BaseGui.h"
#include "ui/renderable/Container.h"

namespace MTGame
{

	class BaseHud : public BaseGui, public MT::Container
	{
	protected:

	public:
		BaseHud(GuiHud hud);

		bool isActive();
	};

}
