#pragma once

#include "ui/renderable/element/Animated.h"

namespace AWTest
{
	class TestActor : public AW::Animated
	{
	public:
		TestActor() { this->registerGameObject<TestActor>(__FUNCTION__); }
	};

}
