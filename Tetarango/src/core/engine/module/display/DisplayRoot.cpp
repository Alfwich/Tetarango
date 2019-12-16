#include "DisplayRoot.h"

namespace AWCore
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setTag(AOTags::IsRootElement, true);
		timeScope = TimeScope::Global;
	}
}
