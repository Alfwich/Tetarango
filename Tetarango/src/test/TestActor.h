#pragma once

#include "ui/renderable/element/Animated.h"

namespace AWTest
{
	class TestActor : public AW::Animated
	{
	public:
		TestActor() { GORegister(TestActor); }
	};

}
