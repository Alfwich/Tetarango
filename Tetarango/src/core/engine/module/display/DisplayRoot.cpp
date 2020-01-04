#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setIsRootElement();
		timeScope = TimeScope::Global;
		GORegister(DisplayRoot);
	}

	void DisplayRoot::onLayoutChildren()
	{
		topLeftAlignSelf();
	}
}
