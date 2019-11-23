#pragma once

#include "ui/renderable/element/Animated.h"

namespace MTTest
{
	class TestActor : public MT::Animated
	{
	public:
		TestActor() {
			this->enableSerialization<TestActor>();
		}
	};

}
