#pragma once

#include "ui/renderable/container/Container.h"

namespace AW
{
	class DisplayRoot : public Container
	{
	public:
		DisplayRoot();

		void onLayoutChildren();
	};
}
