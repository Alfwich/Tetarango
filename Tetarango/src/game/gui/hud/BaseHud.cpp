#include "BaseHud.h"

namespace AWGame
{
	BaseHud::BaseHud(GuiHud hud) : BaseGui(hud) {}

	bool BaseHud::isActive()
	{
		return modules->time->getTimeFactorForScope(AW::TimeScope::Game) > 0.0;
	}
}
