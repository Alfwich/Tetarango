#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setIsRootElement();
		timeScope = TimeScope::Global;
		registerSerialization<DisplayRoot>();
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
