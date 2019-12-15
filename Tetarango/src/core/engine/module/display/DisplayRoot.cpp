#include "DisplayRoot.h"

namespace MT
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setTag(AOTags::IsRootElement, true);
		timeScope = TimeScope::Global;
	}
}
