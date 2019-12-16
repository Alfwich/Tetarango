#pragma once

#include "ui/renderable/element/Animated.h"

namespace MTTest
{
	class TestActor : public AWCore::Animated
	{
	public:
		TestActor() {
			this->enableSerialization<TestActor>();
		}
	};

}
