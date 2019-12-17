#include "DisplayRoot.h"

namespace AW
{
	DisplayRoot::DisplayRoot()
	{
		activate();
		enableInput();
		setTag(ATags::IsRootElement, true);
		timeScope = TimeScope::Global;
	}
}
