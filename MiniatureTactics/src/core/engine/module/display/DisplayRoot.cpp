#include "DisplayRoot.h"

namespace MT
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		inputEnabled = true;
		setTag(AOTags::IsRootElement, true);
		timeScope = TimeScope::Global;
	}
}
