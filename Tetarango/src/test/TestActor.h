#pragma once

#include "ui/renderable/element/Animated.h"

namespace MTTest
{
	class TestActor : public AW::Animated
	{
	public:
		TestActor() {
			this->registerSerialization<TestActor>();
		}
	};

}
