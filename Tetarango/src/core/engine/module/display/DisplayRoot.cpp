#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setIsRootElement();
		timeScope = TimeScope::Global;
		registerGameObject<DisplayRoot>(__FUNCTION__);
	}

	void DisplayRoot::onLayoutChildren()
	{
		topLeftAlignSelf();
	}
}
