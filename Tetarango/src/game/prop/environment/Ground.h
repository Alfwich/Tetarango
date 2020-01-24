#pragma once

#include "ui/renderable/container/Container.h"

namespace AWGame
{
	class Ground : public AW::Container
	{
	public:
		Ground();

		void onCreateChildren() override;
	};
}
