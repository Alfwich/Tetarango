#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setTag(AOTags::IsRootElement, true);
		timeScope = TimeScope::Global;
	}
}
