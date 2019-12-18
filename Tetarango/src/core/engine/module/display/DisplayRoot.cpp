#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setTag(GTags::IsRootElement, true);
		timeScope = TimeScope::Global;
	}

	void DisplayRoot::onInitialAttach()
	{
		setSize(modules->screen->getWidth(), modules->screen->getHeight());
	}

	void DisplayRoot::onLayoutChildren()
	{
		topLeftAlignSelf();
	}
}
