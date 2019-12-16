#pragma once

#include "gui/BaseGui.h"
#include "ui/renderable/container/Container.h"

namespace AWGame
{

	class BaseHud : public BaseGui, public AWCore::Container
	{
	protected:

	public:
		BaseHud(GuiHud hud);

		bool isActive();
	};

}
