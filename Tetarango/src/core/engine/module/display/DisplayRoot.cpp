#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setIsRootElement();
		timeScope = TimeScope::Global;
		registerGameObject<DisplayRoot>("display-root");
	}

	void DisplayRoot::onLayoutChildren()
	{
		topLeftAlignSelf();
	}
}
