#include "BaseHud.h"

namespace MTGame
{
	BaseHud::BaseHud(GuiHud hud) : BaseGui(hud) {}

	bool BaseHud::isActive()
	{
		return modules->time->getTimeFactorForScope(MT::TimeScope::Game) > 0.0;
	}
}
