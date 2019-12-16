#include "BaseHud.h"

namespace AWGame
{
	BaseHud::BaseHud(GuiHud hud) : BaseGui(hud) {}

	bool BaseHud::isActive()
	{
		return modules->time->getTimeFactorForScope(AWCore::TimeScope::Game) > 0.0;
	}
}
