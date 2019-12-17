#pragma once

#include "gui/BaseGui.h"
#include "ui/renderable/container/Container.h"

namespace AWGame
{

	class BaseHud : public BaseGui, public AW::Container
	{
	protected:

	public:
		BaseHud(GuiHud hud);

		bool isActive();
	};

}
